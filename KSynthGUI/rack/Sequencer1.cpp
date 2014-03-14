#include "Sequencer1.h"

#include <iostream>
#include <QPainter>
#include <QSpinBox>

#include <KSynth/notes/Note.h>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QBoxLayout>

#include "Sequencer1Selector.h"
#include "../controls/ImgSpin.h"
#include "../controls/CheckBox.h"
#include "../controls/CheckBoxGroup.h"
#include "../misc/Skin.h"
#include "../misc/Helper.h"

Sequencer1::Sequencer1(Context& ctx, QWidget *parent) :
	RackElement(ctx, parent) {

	setMinimumSize(760,152);
	setMaximumSize(760,152);

	elements.spnBars = new ImgSpin(this);			//elements.spnBars->setMinimum(1);		elements.spnBars->setMaximum(8);		elements.spnBars->setValue(4);
	elements.spnGranularity = new ImgSpin(this);	//elements.spnGranularity->setMinimum(2);	elements.spnGranularity->setMaximum(8);	elements.spnGranularity->setValue(4);
	elements.spnOctave = new ImgSpin(this);			//elements.spnOctave->setMinimum(1);		elements.spnOctave->setMaximum(6);		elements.spnOctave->setValue(2);

	selector = new Sequencer1Selector(*this, this);		selector->setParent(this);

	QGridLayout* lay = new QGridLayout();
	this->setLayout(lay);
	lay->setSizeConstraint(QLayout::SetNoConstraint);

	// pattern selection
	elements.grpPattern = new CheckBoxGroup();
	for (unsigned int i = 0; i < 16; ++i) {
		elements.chkPattern[i] = new CheckBox(this);
		elements.grpPattern->add(elements.chkPattern[i]);
		elements.chkPattern[i]->connectTo(i, this, SLOT(onPatternSelect()));
	}

	this->setStyleSheet("QComboBox { background-color: blue; }");

	connect(elements.spnBars, SIGNAL(onChange()), this, SLOT(onValueChanged()));
	connect(elements.spnGranularity, SIGNAL(onChange()), this, SLOT(onValueChanged()));
	connect(elements.spnOctave, SIGNAL(onChange()), this, SLOT(onValueChanged()));

	for (unsigned int i = 1; i <= 4; ++i) {
		elements.spnBars->add(Helper::getForNumber(i), i);
	}
	for (unsigned int i = 1; i <= 6; ++i) {
		elements.spnOctave->add(Helper::getForNumber(i), i);
	}
	for (unsigned int i = 2; i <= 8; ++i) {
		elements.spnGranularity->add(Helper::getForNumber(i), i);
	}

}

Sequencer1::~Sequencer1() {
	delete elements.grpPattern;
}



//RackElement* Sequencer1::createFromPrototype(Context& ctx) const {
//	return new Sequencer1( (QWidget*) parent() );
//}

void Sequencer1::refresh() {

}

void Sequencer1::onPatternSelect() {

	QObject* obj = sender();

	// cast it to a midi-ui element
	const MidiUI* ui = dynamic_cast<const MidiUI*>(obj);
	if (!ui) {throw "invalid element attached";}

	// get the pattern index
	Param param = ui->getParamType();

	setCurrentPattern(param);
	emit repaint();

}

void Sequencer1::onValueChanged() {
	setNumBars( (unsigned int) elements.spnBars->getSelectedValue());
	setGranulartity( (unsigned int) elements.spnGranularity->getSelectedValue());
	mscaler.setBars(elements.spnBars->getSelectedValue());
	mscaler.setGranularity(elements.spnGranularity->getSelectedValue());
	mscaler.setOctave(elements.spnOctave->getSelectedValue());
	emit repaint();
}

void Sequencer1::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);

	for (unsigned int y = 0; y < 4; ++y) {
		for (unsigned int x = 0; x < 4; ++x) {
			unsigned int idx = y*4 + x;
			elements.chkPattern[idx]->setGeometry( int(672+x*18), int(20+y*18),		0,0);
		}
	}

	elements.spnBars->setGeometry(12,20, 0,0);
	elements.spnGranularity->setGeometry(12,68, 0,0);
	elements.spnOctave->setGeometry(12,116, 0,0);

	selector->setGeometry(120,16, 0,0);

}


void Sequencer1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	//QColor cA(128,128,128);
	//p.setBrush(cA);
	//p.drawRect(0,0,this->width()-1,this->height()-1);

	// background
	static QImage imgBg = Skin::getImage("skin/sequencer1.png", "PNG");
	p.drawImage(0,0, imgBg);
	p.setPen(Qt::NoPen);

	//drawNotes(p);

}

