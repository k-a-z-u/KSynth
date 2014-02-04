#include "Mixer1.h"

#include "../controls/Knob.h"
#include "../controls/Slider1.h"
#include "../controls/VUMeter.h"
#include "../controls/Knob.h"
#include "../controls/LED1.h"
#include "../controls/PinConnector.h"
#include "../controls/TextLabel.h"
#include "../model/Context.h"
#include "../misc/Skin.h"

#include <KSynth/Generator.h>

Mixer1::Mixer1(Context& ctx, QWidget *parent) :
	RackElement(ctx, parent), SimpleMixer() {

	setSize(760, 192);

	elements.connector = new PinConnector(ctx, this, this);

	elements.vuMeter = new VUMeter(this);
	elements.sldMainVol = new Slider1("main", 0, 127, 100, 5, this);
	elements.sldMainVol->addSnap(100, 5);

	for (unsigned int i = 0; i < 9; ++i) {
		elements.gslots[i].sldVolume = new Slider1("vol " + std::to_string(i+1), 0, 127, 100, 5, this);
		elements.gslots[i].sldVolume->addSnap(100, 5);
		elements.gslots[i].vuMeter = new VUMeter(this);
		elements.gslots[i].kHighPass = new Knob(" HP", 0, 100, 0, 5, this);
		elements.gslots[i].kLowPass = new Knob(" LP", 0, 100, 0, 5, this);
		elements.gslots[i].kPan = new Knob("pan", 0, 100, 50, 5, this);
		elements.gslots[i].kPan->addSnap(50, 4);
		elements.gslots[i].ledLowPass = new LED1(this);
		elements.gslots[i].ledHighPass = new LED1(this);
	}

	elements.sldMainVol->connectTo((int)SimpleMixerParams::MASTER_VOLUME, this, SLOT(onParamChange()));

	elements.gslots[0].sldVolume->connectTo((int)SimpleMixerParams::SLOT1_VOLUME, this, SLOT(onParamChange()));
	elements.gslots[1].sldVolume->connectTo((int)SimpleMixerParams::SLOT2_VOLUME, this, SLOT(onParamChange()));
	elements.gslots[2].sldVolume->connectTo((int)SimpleMixerParams::SLOT3_VOLUME, this, SLOT(onParamChange()));
	elements.gslots[3].sldVolume->connectTo((int)SimpleMixerParams::SLOT4_VOLUME, this, SLOT(onParamChange()));
	elements.gslots[4].sldVolume->connectTo((int)SimpleMixerParams::SLOT5_VOLUME, this, SLOT(onParamChange()));
	elements.gslots[5].sldVolume->connectTo((int)SimpleMixerParams::SLOT6_VOLUME, this, SLOT(onParamChange()));
	elements.gslots[6].sldVolume->connectTo((int)SimpleMixerParams::SLOT7_VOLUME, this, SLOT(onParamChange()));
	elements.gslots[7].sldVolume->connectTo((int)SimpleMixerParams::SLOT8_VOLUME, this, SLOT(onParamChange()));
	elements.gslots[8].sldVolume->connectTo((int)SimpleMixerParams::SLOT9_VOLUME, this, SLOT(onParamChange()));

	elements.gslots[0].kHighPass->connectTo((int)SimpleMixerParams::SLOT1_HIGH_PASS, this, SLOT(onParamChange()));
	elements.gslots[1].kHighPass->connectTo((int)SimpleMixerParams::SLOT2_HIGH_PASS, this, SLOT(onParamChange()));
	elements.gslots[2].kHighPass->connectTo((int)SimpleMixerParams::SLOT3_HIGH_PASS, this, SLOT(onParamChange()));
	elements.gslots[3].kHighPass->connectTo((int)SimpleMixerParams::SLOT4_HIGH_PASS, this, SLOT(onParamChange()));
	elements.gslots[4].kHighPass->connectTo((int)SimpleMixerParams::SLOT5_HIGH_PASS, this, SLOT(onParamChange()));
	elements.gslots[5].kHighPass->connectTo((int)SimpleMixerParams::SLOT6_HIGH_PASS, this, SLOT(onParamChange()));
	elements.gslots[6].kHighPass->connectTo((int)SimpleMixerParams::SLOT7_HIGH_PASS, this, SLOT(onParamChange()));
	elements.gslots[7].kHighPass->connectTo((int)SimpleMixerParams::SLOT8_HIGH_PASS, this, SLOT(onParamChange()));
	elements.gslots[8].kHighPass->connectTo((int)SimpleMixerParams::SLOT9_HIGH_PASS, this, SLOT(onParamChange()));

	elements.gslots[0].kLowPass->connectTo((int)SimpleMixerParams::SLOT1_LOW_PASS, this, SLOT(onParamChange()));
	elements.gslots[1].kLowPass->connectTo((int)SimpleMixerParams::SLOT2_LOW_PASS, this, SLOT(onParamChange()));
	elements.gslots[2].kLowPass->connectTo((int)SimpleMixerParams::SLOT3_LOW_PASS, this, SLOT(onParamChange()));
	elements.gslots[3].kLowPass->connectTo((int)SimpleMixerParams::SLOT4_LOW_PASS, this, SLOT(onParamChange()));
	elements.gslots[4].kLowPass->connectTo((int)SimpleMixerParams::SLOT5_LOW_PASS, this, SLOT(onParamChange()));
	elements.gslots[5].kLowPass->connectTo((int)SimpleMixerParams::SLOT6_LOW_PASS, this, SLOT(onParamChange()));
	elements.gslots[6].kLowPass->connectTo((int)SimpleMixerParams::SLOT7_LOW_PASS, this, SLOT(onParamChange()));
	elements.gslots[7].kLowPass->connectTo((int)SimpleMixerParams::SLOT8_LOW_PASS, this, SLOT(onParamChange()));
	elements.gslots[8].kLowPass->connectTo((int)SimpleMixerParams::SLOT9_LOW_PASS, this, SLOT(onParamChange()));

	elements.gslots[0].kPan->connectTo((int)SimpleMixerParams::SLOT1_PAN, this, SLOT(onParamChange()));
	elements.gslots[1].kPan->connectTo((int)SimpleMixerParams::SLOT2_PAN, this, SLOT(onParamChange()));
	elements.gslots[2].kPan->connectTo((int)SimpleMixerParams::SLOT3_PAN, this, SLOT(onParamChange()));
	elements.gslots[3].kPan->connectTo((int)SimpleMixerParams::SLOT4_PAN, this, SLOT(onParamChange()));
	elements.gslots[4].kPan->connectTo((int)SimpleMixerParams::SLOT5_PAN, this, SLOT(onParamChange()));
	elements.gslots[5].kPan->connectTo((int)SimpleMixerParams::SLOT6_PAN, this, SLOT(onParamChange()));
	elements.gslots[6].kPan->connectTo((int)SimpleMixerParams::SLOT7_PAN, this, SLOT(onParamChange()));
	elements.gslots[7].kPan->connectTo((int)SimpleMixerParams::SLOT8_PAN, this, SLOT(onParamChange()));
	elements.gslots[8].kPan->connectTo((int)SimpleMixerParams::SLOT9_PAN, this, SLOT(onParamChange()));


}

//std::string Mixer1::getDeviceType() const {
//	return "Mixer1";
//}

//RackElement* Mixer1::createFromPrototype(Context& ctx) const {
//	return new Mixer1( (QWidget*) parent() );
//}

void Mixer1::onParamChange() {

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
void Mixer1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Skin::getImage("skin/mixer1.png", "PNG");
	p.drawImage(0,0, imgBg);

}

void Mixer1::resizeEvent(QResizeEvent* event) {

	Q_UNUSED(event);

	int s = 64+8;
	for (unsigned int i = 0; i < 9; ++i) {
		int ss = s * i;

		elements.gslots[i].sldVolume->setGeometry(14+ss, 92, 0,0);
		elements.gslots[i].vuMeter->setGeometry(14+32+ss, 92+8, 0,0);

		elements.gslots[i].kHighPass->setGeometry(10+ss,55, 0,0);
		elements.gslots[i].kLowPass->setGeometry(10+ss+30,55, 0,0);

		elements.gslots[i].ledHighPass->setGeometry(9+ss,80, 0,0);
		elements.gslots[i].ledLowPass->setGeometry(9+ss+30,80, 0,0);

		elements.gslots[i].kPan->setGeometry(24+ss,20, 0,0);

	}

	elements.sldMainVol->setGeometry(12+s*9,92, 0,0);
	elements.vuMeter->setGeometry(12+34+s*9,92+8, 0,0);

	elements.connector->setGeometry(728+4,7, 0,0);
	label->setGeometry(723+4,41, 32, 94);

}

void Mixer1::refresh() {

	//elements.vuMeter->setValue(getVU());

	for (unsigned int i = 0; i < 9; ++i) {
//		BindingEndpoint src = ctx.gen->getBinder().getBindingToInputChannel(this, i*2);
//		if (!src.dev) {continue;}
//		elements.gslots[i].vuMeter->setValue(src.dev->getVU());
		elements.gslots[i].ledHighPass->setValue(isHPActive(i));
		elements.gslots[i].ledLowPass->setValue(isLPActive(i));
		elements.gslots[i].vuMeter->setValue(getVU(i));
	}

	elements.vuMeter->setValue(getVU());
	elements.sldMainVol->setValueFromParam(getParameter((int)SimpleMixerParams::MASTER_VOLUME));

	elements.gslots[0].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT1_VOLUME));
	elements.gslots[1].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT2_VOLUME));
	elements.gslots[2].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT3_VOLUME));
	elements.gslots[3].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT4_VOLUME));
	elements.gslots[4].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT5_VOLUME));
	elements.gslots[5].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT6_VOLUME));
	elements.gslots[6].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT7_VOLUME));
	elements.gslots[7].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT8_VOLUME));
	elements.gslots[8].sldVolume->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT9_VOLUME));

	elements.gslots[0].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT1_HIGH_PASS));
	elements.gslots[1].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT2_HIGH_PASS));
	elements.gslots[2].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT3_HIGH_PASS));
	elements.gslots[3].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT4_HIGH_PASS));
	elements.gslots[4].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT5_HIGH_PASS));
	elements.gslots[5].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT6_HIGH_PASS));
	elements.gslots[6].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT7_HIGH_PASS));
	elements.gslots[7].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT8_HIGH_PASS));
	elements.gslots[8].kHighPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT9_HIGH_PASS));

	elements.gslots[0].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT1_LOW_PASS));
	elements.gslots[1].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT2_LOW_PASS));
	elements.gslots[2].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT3_LOW_PASS));
	elements.gslots[3].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT4_LOW_PASS));
	elements.gslots[4].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT5_LOW_PASS));
	elements.gslots[5].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT6_LOW_PASS));
	elements.gslots[6].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT7_LOW_PASS));
	elements.gslots[7].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT8_LOW_PASS));
	elements.gslots[8].kLowPass->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT9_LOW_PASS));

	elements.gslots[0].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT1_PAN));
	elements.gslots[1].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT2_PAN));
	elements.gslots[2].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT3_PAN));
	elements.gslots[3].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT4_PAN));
	elements.gslots[4].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT5_PAN));
	elements.gslots[5].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT6_PAN));
	elements.gslots[6].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT7_PAN));
	elements.gslots[7].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT8_PAN));
	elements.gslots[8].kPan->setValueFromParam(getParameter((int)SimpleMixerParams::SLOT9_PAN));


}
