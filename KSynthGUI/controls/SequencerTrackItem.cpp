#include "SequencerTrackItem.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>

#include <KSynth/notes/NoteDevice.h>

#include "model/Context.h"
#include "../rack/Rack.h"
#include "../rack/RackElement.h"
#include "../model/RackElementModel.h"
#include "../misc/Helper.h"
#include "SequencerTracksWidget.h"

SequencerTrackItem::SequencerTrackItem(SequencerTracksWidget& tracks, Rack& rack, SequencerTrack& st, QWidget *parent) :
	QWidget(parent), tracks(tracks), rack(rack), focused(false), st(st) {

	//cmbMdl = new RackElementModel(rack);

	setFocusPolicy(Qt::ClickFocus);

	// tool tip
	setToolTip( st.getDescription().c_str() );

	// item's size
	setMinimumSize(192, 40);
	setMaximumSize(192, 40);

	// change font size
	QFont font;
	font.setPointSize(font.pointSize() - 1);

	// the track's name
	txtName = new QLineEdit(this);
	txtName->setFont(font);
	txtName->setText(st.getName().c_str());
	txtName->setStyleSheet("QLineEdit { border:none; background-color:rgba(0,0,0,0); }");

	// the track's device to use for synthesizing
	cmbDevice = new QComboBox(this);
	//cmbDevice->setModel(cmbMdl);
	//cmbDevice->setMaximumSize(75, 20);
	cmbDevice->setFont(font);

	// muting
	chkMute = new QCheckBox(this);
	chkMute->setText("mute");
	chkMute->setFont(font);

	// signals
	connect(&rack, SIGNAL(onRackChanged()), this, SLOT(updateCombo()));
	connect(txtName, SIGNAL(editingFinished()), this, SLOT(onChangeTrackName()));
	connect(cmbDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeTargetDevice()));

	updateCombo();

}

SequencerTrackItem::~SequencerTrackItem() {
	//delete cmbMdl;
}

SequencerTrack& SequencerTrackItem::getTrack() {
	return st;
}

void SequencerTrackItem::focusInEvent(QFocusEvent* e) {
	Q_UNUSED(e);
//	this->focused = true;
//	emit repaint();
	tracks.setSeleceted(*this);
}

void SequencerTrackItem::focusOutEvent(QFocusEvent* e) {
	Q_UNUSED(e);
//	this->focused = false;
//	emit repaint();
}


void SequencerTrackItem::setSelected(bool sel) {
	bool changed = focused != sel;
	this->focused = sel;
	if (changed) {emit repaint();}
}

void SequencerTrackItem::updateCombo() {

	cmbDevice->blockSignals(true); {

		cmbDevice->clear();

		// add "deselection" entry
		cmbDevice->addItem("<none>", qVariantFromValue( (void*) nullptr ));
		cmbDevice->setCurrentIndex(0);

		// add elements
		for (NoteDevice* dev : rack.getNoteDevices()) {
			cmbDevice->addItem(dev->getUserName().c_str(), qVariantFromValue( (void*) dev ));
			if (st.getDevice() == dev) {cmbDevice->setCurrentIndex( cmbDevice->count() - 1 );}
		}

	} cmbDevice->blockSignals(false);

}


#include <QPainter>
void SequencerTrackItem::paintEvent (QPaintEvent* event) {
	Q_UNUSED(event);
	QPainter p(this);
	static QImage imgBg = Helper::getSkinImage("skin/sequencerTrack.png", "PNG");

	if (focused) {
		p.setBrush(QColor(192,192,255));
	} else {
		p.setBrush(QColor(255,255,255));
	}
	p.drawRect(0,0,width(),height());
	p.drawImage(0,0, imgBg);

}


void SequencerTrackItem::resizeEvent(QResizeEvent* event) {

	Q_UNUSED(event);

	txtName->setGeometry(6,4, 96,20);
	cmbDevice->setGeometry(96+6+4,4, 84,20);
	chkMute->setGeometry(2,28, 64,8);

}

void SequencerTrackItem::onChangeTrackName() {
	st.setName(txtName->text().toStdString());
}

void SequencerTrackItem::onChangeTargetDevice() {
	QVariant var = cmbDevice->itemData(cmbDevice->currentIndex(), Qt::UserRole);
	void* ptr = var.value<void*>();
	if (ptr == 0) {
		st.setDevice(nullptr);
	} else {
		NoteDevice* dev = reinterpret_cast<NoteDevice*>(ptr);
		//dev = (NoteDevice*) RackElement::getElements().at(cmbDevice->currentIndex());
		if (dev == nullptr) {throw "error while casting void* to NoteDevice";}
		st.setDevice(dev);
	}
}
