#include "Synth1.h"


#include <QLayout>
#include <QPainter>
#include <QImage>

#include "../misc/Helper.h"

#include "../controls/Knob.h"
#include "../controls/Slider1.h"
#include "../controls/ImgSpin.h"
#include "../controls/CheckBox.h"
#include "../controls/VUMeter.h"
#include "../controls/PinConnector.h"
#include "../controls/TextLabel.h"

Synth1::Synth1(Context& ctx, QWidget *parent) :
	RackElement(ctx, parent) {

	setSize(760,192);

	//elements.label = new TextLabel(this);
	elements.con = new PinConnector(ctx, this, this);

	elements.kFilterFreq = new Knob("freq", 0, 100, 0, this);
	elements.kFilterRes = new Knob("res", 0, 100, 0, this);
	elements.kMix = new Knob("mix", 0, 100, 50, this);
	elements.kMix->addSnap(50, 3);
	elements.sldVolume = new Slider1("vol", 0, 127, 0, this);
	elements.sldVolume->addSnap(100, 5);

	elements.gosc[0].kFineOffset = new Knob("fine", -10, 10, 0, this);
	elements.gosc[0].kOctave = new Knob("octave", 0, 8, 0, this);
	elements.gosc[0].kSemi = new Knob("semi", 0, 11, 0, this);
	elements.gosc[0].chkKey = new CheckBox("kbd", true, this);
	elements.gosc[0].kPhase = new Knob("phase", 0, 100, 0, this);

	elements.gosc[1].kFineOffset = new Knob("fine", -10, 10, 0, this);
	elements.gosc[1].kOctave = new Knob("octave", 0, 8, 0, this);
	elements.gosc[1].kSemi = new Knob("semi", 0, 11, 0, this);
	elements.gosc[1].chkKey = new CheckBox("kbd", true, this);
	elements.gosc[1].kPhase = new Knob("phase", 0, 100, 0, this);

	elements.oscHuell.sA = new Slider1("A", 0, 100, 5, this);
	elements.oscHuell.sD = new Slider1("D", 0, 100, 5, this);
	elements.oscHuell.sS = new Slider1("S", 0, 100, 75, this);
	elements.oscHuell.sR = new Slider1("R", 0, 1000, 50, this);

	elements.filterHuell.sA = new Slider1("A", 0, 100, 5, this);
	elements.filterHuell.sD = new Slider1("D", 0, 100, 5, this);
	elements.filterHuell.sS = new Slider1("S", 0, 100, 75, this);
	elements.filterHuell.sR = new Slider1("R", 0, 1000, 50, this);

	elements.gosc[0].spnMode = new ImgSpin(this);
	elements.gosc[1].spnMode = new ImgSpin(this);

	elements.glfo[0].spnMode = new ImgSpin(this);
	elements.glfo[0].kFreq = new Knob("freq", 0, 100, 0, this);
	elements.glfo[0].kAmount = new Knob("amount", 0, 100, 0, this);


	elements.cRingMod = new CheckBox("rmod", true, this);

	for (int i = 0; i < (int) SimpleOscillator2Mode::_END; ++i) {
		elements.gosc[0].spnMode->add(Helper::getForOscFunction((SimpleOscillator2Mode) i, 30,24), 0);
		elements.gosc[1].spnMode->add(Helper::getForOscFunction((SimpleOscillator2Mode) i, 30,24), 0);
		elements.glfo[0].spnMode->add(Helper::getForOscFunction((SimpleOscillator2Mode) i, 30,24), 0);
	}

	elements.vuMeter = new VUMeter(this);

	elements.oscHuell.sA->connectTo((int) SimpleSynthParams::OSC_HUELL_A, this, SLOT (onParamChange()));
	elements.oscHuell.sD->connectTo((int) SimpleSynthParams::OSC_HUELL_D, this, SLOT (onParamChange()));
	elements.oscHuell.sS->connectTo((int) SimpleSynthParams::OSC_HUELL_S, this, SLOT (onParamChange()));
	elements.oscHuell.sR->connectTo((int) SimpleSynthParams::OSC_HUELL_R, this, SLOT (onParamChange()));

	elements.filterHuell.sA->connectTo((int) SimpleSynthParams::FILTER_HUELL_A, this, SLOT (onParamChange()));
	elements.filterHuell.sD->connectTo((int) SimpleSynthParams::FILTER_HUELL_D, this, SLOT (onParamChange()));
	elements.filterHuell.sS->connectTo((int) SimpleSynthParams::FILTER_HUELL_S, this, SLOT (onParamChange()));
	elements.filterHuell.sR->connectTo((int) SimpleSynthParams::FILTER_HUELL_R, this, SLOT (onParamChange()));

	elements.gosc[0].chkKey->connectTo((int) SimpleSynthParams::OSC1_USE_KEYBOARD, this, SLOT (onParamChange()));
	elements.gosc[1].chkKey->connectTo((int) SimpleSynthParams::OSC2_USE_KEYBOARD, this, SLOT (onParamChange()));

	elements.gosc[0].spnMode->connectTo((int) SimpleSynthParams::OSC1_MODE, this, SLOT (onParamChange()));
	elements.gosc[1].spnMode->connectTo((int) SimpleSynthParams::OSC2_MODE, this, SLOT (onParamChange()));

	elements.gosc[0].kOctave->connectTo((int) SimpleSynthParams::OSC1_OCTAVE, this, SLOT (onParamChange()));
	elements.gosc[1].kOctave->connectTo((int) SimpleSynthParams::OSC2_OCTAVE, this, SLOT (onParamChange()));

	elements.gosc[0].kSemi->connectTo((int) SimpleSynthParams::OSC1_SEMI, this, SLOT (onParamChange()));
	elements.gosc[1].kSemi->connectTo((int) SimpleSynthParams::OSC2_SEMI, this, SLOT (onParamChange()));

	elements.gosc[0].kFineOffset->connectTo((int) SimpleSynthParams::OSC1_FINE, this, SLOT (onParamChange()));
	elements.gosc[1].kFineOffset->connectTo((int) SimpleSynthParams::OSC2_FINE, this, SLOT (onParamChange()));

	elements.gosc[0].kPhase->connectTo((int) SimpleSynthParams::OSC1_PHASE_OFFSET, this, SLOT (onParamChange()));
	elements.gosc[1].kPhase->connectTo((int) SimpleSynthParams::OSC2_PHASE_OFFSET, this, SLOT (onParamChange()));

	elements.glfo[0].kAmount->connectTo((int) SimpleSynthParams::LFO1_AMOUNT, this, SLOT(onParamChange()));
	elements.glfo[0].kFreq->connectTo((int) SimpleSynthParams::LFO1_FREQUENCY, this, SLOT(onParamChange()));
	elements.glfo[0].spnMode->connectTo((int) SimpleSynthParams::LFO1_MODE, this, SLOT(onParamChange()));

	elements.kMix->connectTo((int) SimpleSynthParams::OSC_MIX, this, SLOT (onParamChange()));
	elements.cRingMod->connectTo((int) SimpleSynthParams::USE_RING_MODULATION, this, SLOT (onParamChange()));
	elements.sldVolume->connectTo((int) SimpleSynthParams::OUTPUT_VOLUME, this, SLOT (onParamChange()));

	elements.kFilterFreq->connectTo((int) SimpleSynthParams::FILTER_FREQUENCY, this, SLOT (onParamChange()));
	elements.kFilterRes->connectTo((int) SimpleSynthParams::FILTER_RESONANCE, this, SLOT (onParamChange()));

	//connect(elements.label, SIGNAL(valueChanged()), this, SIGNAL(userNameChanged()));

	refresh();

}

//RackElement* Synth1::createFromPrototype(Context& ctx) const {
//	return new Synth1( (QWidget*) parent() );
//}

//std::string Synth1::getDeviceType() const {
//	return "Synth1";
//}

void Synth1::onParamChange() {

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


void Synth1::refresh() {

	elements.vuMeter->setValue(getVU());

	elements.gosc[0].spnMode->setValueFromParam(getParameter(elements.gosc[0].spnMode->getParamType()));
	elements.gosc[1].spnMode->setValueFromParam(getParameter(elements.gosc[1].spnMode->getParamType()));

	elements.gosc[0].kSemi->setValueFromParam(getParameter(elements.gosc[0].kSemi->getParamType()));
	elements.gosc[1].kSemi->setValueFromParam(getParameter(elements.gosc[1].kSemi->getParamType()));

	elements.gosc[0].kOctave->setValueFromParam(getParameter(elements.gosc[0].kOctave->getParamType()));
	elements.gosc[1].kOctave->setValueFromParam(getParameter(elements.gosc[1].kOctave->getParamType()));

	elements.gosc[0].kFineOffset->setValueFromParam(getParameter(elements.gosc[0].kFineOffset->getParamType()));
	elements.gosc[1].kFineOffset->setValueFromParam(getParameter(elements.gosc[1].kFineOffset->getParamType()));

	elements.gosc[0].chkKey->setValueFromParam(getParameter(elements.gosc[0].chkKey->getParamType()));
	elements.gosc[1].chkKey->setValueFromParam(getParameter(elements.gosc[1].chkKey->getParamType()));

	elements.gosc[0].kPhase->setValueFromParam(getParameter(elements.gosc[0].kPhase->getParamType()));
	elements.gosc[1].kPhase->setValueFromParam(getParameter(elements.gosc[1].kPhase->getParamType()));

	elements.glfo[0].kAmount->setValueFromParam(getParameter(elements.glfo[0].kAmount->getParamType()));
	elements.glfo[0].kFreq->setValueFromParam(getParameter(elements.glfo[0].kFreq->getParamType()));
	elements.glfo[0].spnMode->setValueFromParam(getParameter(elements.glfo[0].spnMode->getParamType()));

	elements.oscHuell.sA->setValueFromParam(getParameter(elements.oscHuell.sA->getParamType()));
	elements.oscHuell.sD->setValueFromParam(getParameter(elements.oscHuell.sD->getParamType()));
	elements.oscHuell.sS->setValueFromParam(getParameter(elements.oscHuell.sS->getParamType()));
	elements.oscHuell.sR->setValueFromParam(getParameter(elements.oscHuell.sR->getParamType()));

	elements.filterHuell.sA->setValueFromParam(getParameter(elements.filterHuell.sA->getParamType()));
	elements.filterHuell.sD->setValueFromParam(getParameter(elements.filterHuell.sD->getParamType()));
	elements.filterHuell.sS->setValueFromParam(getParameter(elements.filterHuell.sS->getParamType()));
	elements.filterHuell.sR->setValueFromParam(getParameter(elements.filterHuell.sR->getParamType()));

	elements.sldVolume->setValueFromParam(getParameter(elements.sldVolume->getParamType()));
	elements.cRingMod->setValueFromParam(getParameter(elements.cRingMod->getParamType()));

	elements.kFilterFreq->setValueFromParam(getParameter(elements.kFilterFreq->getParamType()));
	elements.kFilterRes->setValueFromParam(getParameter(elements.kFilterRes->getParamType()));

}

void Synth1::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);

	const int ADSRdist = 30;

	elements.kFilterFreq->setGeometry(462,20, 0,0);
	elements.kFilterRes->setGeometry(462,55, 0,0);

	elements.sldVolume->setGeometry(652,12-8, 0,0);
	elements.vuMeter->setGeometry(652+32,12, 0,0);

	elements.oscHuell.sA->setGeometry(325+ADSRdist*0,6, 0,0);
	elements.oscHuell.sD->setGeometry(325+ADSRdist*1,6, 0,0);
	elements.oscHuell.sS->setGeometry(325+ADSRdist*2,6, 0,0);
	elements.oscHuell.sR->setGeometry(325+ADSRdist*3,6, 0,0);

	elements.filterHuell.sA->setGeometry(517+ADSRdist*0,6, 0,0);
	elements.filterHuell.sD->setGeometry(517+ADSRdist*1,6, 0,0);
	elements.filterHuell.sS->setGeometry(517+ADSRdist*2,6, 0,0);
	elements.filterHuell.sR->setGeometry(517+ADSRdist*3,6, 0,0);

	elements.gosc[0].spnMode->setGeometry(16,16, 50,20);
	elements.gosc[1].spnMode->setGeometry(16,64, 50,20);
	elements.glfo[0].spnMode->setGeometry(16,112, 50,20);

	int sx = 70;
	int s = 32;

	elements.gosc[0].kOctave->setGeometry(	sx+s*0,	8,		0,0);
	elements.gosc[1].kOctave->setGeometry(	sx+s*0,	56,		0,0);
	elements.glfo[0].kFreq->setGeometry(	sx+s*0,	104,	0,0);

	elements.glfo[0].kAmount->setGeometry(	sx+s*1,	104,	0,0);

	elements.gosc[0].kSemi->setGeometry(	sx+s*1,	8,		0,0);
	elements.gosc[1].kSemi->setGeometry(	sx+s*1,	56,		0,0);

	elements.gosc[0].kFineOffset->setGeometry(sx+s*2,	8,  0,0);
	elements.gosc[1].kFineOffset->setGeometry(sx+s*2,	56, 0,0);

	sx += 8;

	elements.gosc[0].kPhase->setGeometry(sx+s*3,	8,		0,0);
	elements.gosc[1].kPhase->setGeometry(sx+s*3,	56,		0,0);

	elements.gosc[0].chkKey->setGeometry(sx+s*4,	8+4,	0,0);
	elements.gosc[1].chkKey->setGeometry(sx+s*4,	56+4,	0,0);

	elements.kMix->setGeometry(267,56, 0,0);
	elements.cRingMod->setGeometry(262,20, 20,20);

	elements.con->setGeometry(727+4,8, 0,0);
	label->setGeometry(723+4,45, 32,92);

}



void Synth1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Helper::getSkinImage("skin/synth1.png", "PNG");
	p.drawImage(0,0, imgBg);

	//QColor cA(128,128,128);
	//p.setBrush(cA);
	//p.drawRect(0,0,this->width()-1,this->height()-1);

}
