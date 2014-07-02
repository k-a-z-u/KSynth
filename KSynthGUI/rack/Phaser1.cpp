#include "Phaser1.h"

#include "../controls/PinConnector.h"
#include "../controls/Knob.h"
#include "../controls/ImgSpin.h"
#include "../misc/Skin.h"
#include "../misc/Helper.h"

Phaser1::Phaser1(Context& ctx, QWidget *parent) :
	RackElement(ctx, parent), Phaser(){

	setSize(760, 64);

	elements.connector = new PinConnector(ctx, this, this);
	elements.kMax = new Knob("max", 0, 100, 0, 5, this);
	elements.kMin = new Knob("min", 0, 100, 0, 5, this);
	elements.kSize = new Knob("size", 0, 100, 0, 5, this);
	elements.kFreq = new Knob("freq", 0, 100, 0, 5, this);
	elements.kDryWet = new Knob("dry/wet", 0, 100, 0, 5, this);
	elements.spnMode = new ImgSpin(this);

	elements.kMax->connectTo((int)PhaserParams::FILTER_MAX, this, SLOT(onParamChange()));
	elements.kMin->connectTo((int)PhaserParams::FILTER_MIN, this, SLOT(onParamChange()));
	elements.kSize->connectTo((int)PhaserParams::FILTER_SIZE, this, SLOT(onParamChange()));
	elements.kFreq->connectTo((int)PhaserParams::OSC_FREQ, this, SLOT(onParamChange()));
	elements.kDryWet->connectTo((int)PhaserParams::DRY_WET, this, SLOT(onParamChange()));
	elements.spnMode->connectTo((int)PhaserParams::OSC_FUNCTION, this, SLOT(onParamChange()));

	// available waveforms
	for (int i = 0; i < (int) SimpleOscillator2Mode::_END; ++i) {
		elements.spnMode->add(Helper::getForOscFunction((SimpleOscillator2Mode) i, 30,24), 0);
	}

}


void Phaser1::onParamChange() {

	// get the signal sender
	QObject* obj = QObject::sender();

	// cast it to a midi-ui element
	const MidiUI* ui = dynamic_cast<const MidiUI*>(obj);
	if (!ui) {throw "invalid element attached";}

	// get the new value to set (between [0,1])
	Param param = ui->getParamType();
	ParamValue val = ui->getValueAsParam();

	// set
	setParameter(param, val);

}

#include <QPainter>
void Phaser1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Skin::getImage("skin/mixer1.png", "PNG");
	p.drawImage(0,0, imgBg);

}

void Phaser1::resizeEvent(QResizeEvent* event) {

	Q_UNUSED(event);

	int y = 8;
	int s = 32;
	elements.kMin->setGeometry(s*1, y, 0,0);
	elements.kMax->setGeometry(s*2, y, 0,0);
	elements.kSize->setGeometry(s*3, y, 0,0);
	elements.kFreq->setGeometry(s*4, y, 0,0);
	elements.kDryWet->setGeometry(s*5, y, 0,0);
	elements.spnMode->setGeometry(s*6, y, 50,20);

}

void Phaser1::refresh() {

	elements.kMin->setValueFromParam(getParameter(PhaserParams::FILTER_MIN));
	elements.kMax->setValueFromParam(getParameter(PhaserParams::FILTER_MAX));
	elements.kSize->setValueFromParam(getParameter(PhaserParams::FILTER_SIZE));
	elements.kFreq->setValueFromParam(getParameter(PhaserParams::OSC_FREQ));
	elements.kDryWet->setValueFromParam(getParameter(PhaserParams::DRY_WET));
	elements.spnMode->setValueFromParam(getParameter(PhaserParams::OSC_FUNCTION));

}
