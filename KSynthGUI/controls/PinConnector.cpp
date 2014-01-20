#include "PinConnector.h"
#include "../rack/RackElement.h"

#include "../model/Context.h"
#include "../rack/Rack.h"
#include "../rack/RackElement.h"
#include "../misc/Skin.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLine>
#include <QDialog>
#include <QGroupBox>
#include <QMessageBox>
#include <QMenu>

#include <KSynth/misc/SoundBase.h>
#include <KSynth/Generator.h>


PinConEntry::PinConEntry(Context& ctx, BindingEndpoint src, PinConEntryEdit dir, QWidget* parent) :
	QWidget(parent), ctx(ctx), src(src), dir(dir), curEndpoint(0, nullptr) {



	QHBoxLayout* lay = new QHBoxLayout(this);
	lay->setMargin(2);
	lay->setSpacing(2);

	cmbDst = new QComboBox();
	cmbDstPort = new QComboBox();
	lblName = new QLabel();

	lblName->setMinimumWidth(120);
	cmbDst->setMinimumWidth(120);
	cmbDstPort->setMinimumWidth(120);



	lay->addWidget(lblName);
	lay->addWidget(cmbDst);
	lay->addWidget(cmbDstPort);

	// read pin name for displaying
	PinProperties p;
	if (dir == PinConEntryEdit::ATTACH_OUTPUT) {
		src.dev->getInputProperties(src.idx, &p);
	} else {
		src.dev->getOutputProperties(src.idx, &p);
	}

	lblName->setText(p.name.c_str());

	// register events
	connect(cmbDst, SIGNAL(currentIndexChanged(int)), this, SLOT(onDstSel()));
	connect(cmbDstPort, SIGNAL(currentIndexChanged(int)), this, SLOT(onDstPortSel()));

	fetchTargets();

}

void PinConEntry::fetchTargets() {

	// deselect-item
	cmbDst->clear();
	cmbDst->addItem("----");

	// get current bindings to this channel (if any)
	if (dir == PinConEntryEdit::ATTACH_OUTPUT)	{curEndpoint = ctx.getGenerator()->getBinder().getBindingToInputChannel(src);}
	if (dir == PinConEntryEdit::ATTACH_INPUT)	{curEndpoint = ctx.getGenerator()->getBinder().getBindingToOutputChannel(src);}

	// create list of available endpoints
	for (SoundBase* sbEP : ctx.getRack()->getSoundBaseDevices()) {

		if (dir == PinConEntryEdit::ATTACH_OUTPUT && sbEP->getNumOutputs() == 0)	{continue;}
		if (dir == PinConEntryEdit::ATTACH_INPUT  && sbEP->getNumInputs() == 0)		{continue;}

		// add to combo-box
		RackElement* reEP = dynamic_cast<RackElement*>(sbEP);
		cmbDst->addItem(sbEP->getUserName().c_str(), qVariantFromValue( (void*) reEP ));

		// select?
		if (curEndpoint.dev == sbEP) {cmbDst->setCurrentIndex(cmbDst->count() - 1);}

	}

}

void PinConEntry::onDstSel() {

	// temporarily disable events to avoid heavy pin-switching
	cmbDstPort->blockSignals(true);

	cmbDstPort->clear();

	// get selected endpoint device from first combo box
	RackElement* reEP = (RackElement*) cmbDst->itemData(cmbDst->currentIndex()).value<void*>();
	if (!reEP) {return;}
	SoundBase* sbEP = dynamic_cast<SoundBase*>(reEP);
	cmbDstPort->addItem("----", -1);
	PinProperties props;

	// list available pins for this device
	if (dir == PinConEntryEdit::ATTACH_INPUT) {
		for (unsigned int i = 0; i < sbEP->getNumInputs(); ++i) {
			sbEP->getInputProperties(i, &props);
			cmbDstPort->addItem(props.name.c_str(), QVariant(i));
			if (curEndpoint.dev == sbEP && curEndpoint.idx == i) {cmbDstPort->setCurrentIndex(cmbDstPort->count()-1);}
		}
	} else {
		for (unsigned int i = 0; i < sbEP->getNumOutputs(); ++i) {
			sbEP->getOutputProperties(i, &props);
			cmbDstPort->addItem(props.name.c_str(), QVariant(i));
			if (curEndpoint.dev == sbEP && curEndpoint.idx == i) {cmbDstPort->setCurrentIndex(cmbDstPort->count()-1);}
		}
	}

	// re-enable disabled events
	cmbDstPort->blockSignals(false);

}

void PinConEntry::onDstPortSel() {

	// new target device
	RackElement* reEP = (RackElement*) cmbDst->itemData(cmbDst->currentIndex()).value<void*>();
	if (!reEP) {return;}
	SoundBase* sbEP = dynamic_cast<SoundBase*>(reEP);

	// new target port
	int idxEP = cmbDstPort->itemData(cmbDstPort->currentIndex()).toInt();

	// delete old binding
	ctx.getGenerator()->getBinder().deleteBinding(src.idx, src.dev, curEndpoint.idx, curEndpoint.dev);

	// try to create new binding
	try {
		if (idxEP >= 0) {
			ctx.getGenerator()->getBinder().addBinding(src.idx, src.dev,  idxEP, sbEP);
		}
	} catch (BindingException& e) {
		QMessageBox::critical(this, "error while creating new binding", e.what());
	}

	// refresh gui to ensure new binding was created sucessfully
	//emit onDstSel();
	fetchTargets();

}


PinConWindow::PinConWindow(Context& ctx, RackElement* re, QWidget* parent) : QDialog(parent) {

	setWindowTitle( ("edit bindings for '" + re->getUserName() + "'").c_str() );

	setModal(true);
	QGridLayout* lay = new QGridLayout(this);
	lay->setMargin(5);
	lay->setSpacing(5);

	QGroupBox* grp1 = new QGroupBox(this); grp1->setTitle("inputs");
	QVBoxLayout* lay1 = new QVBoxLayout(grp1);
	lay1->setMargin(0); lay1->setSpacing(0);
	SoundBase* sb = dynamic_cast<SoundBase*>(re);
	for (unsigned int i = 0; i < sb->getNumInputs(); ++i) {
		PinConEntry* pce = new PinConEntry(ctx, BindingEndpoint(i, sb), PinConEntryEdit::ATTACH_OUTPUT, parent);
		lay1->addWidget(pce);
	}
	lay->addWidget(grp1, 0, 0);

	QGroupBox* grp2 = new QGroupBox(this); grp2->setTitle("outputs");
	QVBoxLayout* lay2 = new QVBoxLayout(grp2);
	lay2->setMargin(0); lay2->setSpacing(0);
	for (unsigned int i = 0; i < sb->getNumOutputs(); ++i) {
		PinConEntry* pce = new PinConEntry(ctx, BindingEndpoint(i, sb), PinConEntryEdit::ATTACH_INPUT, parent);
		lay2->addWidget(pce);
	}
	lay->addWidget(grp2, 0, 1);

}
PinConWindow::~PinConWindow() {

}


PinConnector::PinConnector(Context& ctx, RackElement* re, QWidget *parent) :
	QWidget(parent), ctx(ctx), re(re) {

	setMinimumSize(24, 24);
	setMaximumSize(24, 24);

}

PinConnector::~PinConnector() {
	;
}

#include <QPainter>
void PinConnector::paintEvent (QPaintEvent* event) {
	Q_UNUSED(event);
	static QImage imgBG = Skin::getImage("skin/xlr.png", "PNG");
	QPainter p(this);
	p.drawImage(0,0,imgBG);
}

void PinConnector::mouseReleaseEvent (QMouseEvent* e) {
	Q_UNUSED(e);
	startConnect();
}

void PinConnector::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);
}

void PinConnector::startConnect() {

	PinConWindow* pcw = new PinConWindow(ctx, re);
	pcw->show();
	return;

}

//std::string getCurDestination(SoundBase* sb, unsigned int idx) {
//	BindingEndpoint dst = sb->getBindingDestination(idx);
//	if (!dst.dev) {return "none";}
//	PinProperties props;
//	dst.dev->getInputProperties(dst.idx, &props);
//	RackElement* re = dynamic_cast<RackElement*>(dst.dev);
//	return re->getUserName() + ": " + props.name;
//}

//#include <QMessageBox>
//void PinConnector::onConnect() {

//	// get values
//	QObject* obj = sender();
//	SoundBase* sbSrc = (SoundBase*) obj->property("SRC_PTR").value<void*>();
//	SoundBase* sbDst = (SoundBase*) obj->property("DST_PTR").value<void*>();
//	unsigned int iSrc = obj->property("SRC_IDX").toInt();
//	unsigned int iDst = obj->property("DST_IDX").toInt();

//	// add binding
//	try {
//		sbSrc->addBinding(iSrc, sbSrc, iDst, sbDst);
//	} catch (BindingException& be) {
//		QMessageBox::critical(nullptr, "Cannot add binding", be.what());
//	}

//}

//void PinConnector::startConnect() {

//	SoundBase* sbSrc = dynamic_cast<SoundBase*>(re);

//	QMenu myMenu(this);
//	QMenu* mnuOut = myMenu.addMenu("outputs");

//	// get all output pins
//	PinProperties props;
//	for (unsigned int i = 0; i < sbSrc->getNumOutputs(); ++i) {

//		// get pin's name and create menu
//		sbSrc->getOutputProperties(i, &props);
//		QMenu* mnuChannel = mnuOut->addMenu(props.name.c_str());

//		// add info about current pin destination
//		std::string curDst = getCurDestination(sbSrc, i);
//		QAction* actDst = mnuChannel->addAction( ("current destination: " + curDst).c_str());
//		actDst->setEnabled(false);
//		mnuChannel->addSeparator();

//		// fetch a list of all available target devices
//		std::vector<SoundBase*> sbs = ctx.getRack()->getSoundBaseDevices();
//		for (SoundBase* sbDst : sbs) {

//			RackElement* reDst = dynamic_cast<RackElement*>(sbDst);

//			// create entry for target device
//			QMenu* mnuDev = mnuChannel->addMenu(reDst->getUserName().c_str());

//			// fetch a list of all available destination pins
//			for (unsigned int j = 0; j < sbDst->getNumInputs(); ++j) {

//				// get pin's name and create action
//				bool pinValid = sbDst->getInputProperties(j, &props);
//				if (!pinValid) {continue;}
//				QAction* act = mnuDev->addAction(props.name.c_str());
//				act->setProperty("SRC_PTR", qVariantFromValue((void*)sbSrc) );
//				act->setProperty("DST_PTR", qVariantFromValue((void*)sbDst) );
//				act->setProperty("SRC_IDX", i);
//				act->setProperty("DST_IDX", j);
//				connect(act, SIGNAL(triggered()), this, SLOT(onConnect()));

//			}

//		}

//	}

//	myMenu.exec( this->mapToGlobal( QPoint(0,0) ) );

//}
