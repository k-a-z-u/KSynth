#include "Reverb1.h"

#include <KSynth/fx/Reverb.h>
#include "../controls/Knob.h"
#include "../misc/Skin.h"
#include "../controls/LCD.h"
#include "../controls/PinConnector.h"
#include <QPushButton>


Reverb1::Reverb1(Context& ctx, QWidget *parent) :
	RackElement(ctx, parent), AllPassReverb() {

	connector = new PinConnector(ctx, this, this);

	setMinimumSize(760,64);
	setMaximumSize(760,64);

	for (unsigned int i = 0; i < 4; ++i) {
		elements.gates[i].kDelay = new Knob("delay", 0, 100, 30, 5, this);
		elements.gates[i].kPosGain = new Knob("gain+", 0, 100, 0, 5, this);
		elements.gates[i].kNegGain = new Knob("gain-", 0, 100, 0, 5, this);
	}
	elements.kDryWet = new Knob("dry/wet", 0, 100, 50, 5, this);
	elements.kDryWet->connectTo((int)AllPassReverbParams::DRY_WET, this, SLOT(onParamChange()));

	elements.gates[0].kDelay->connectTo((int)AllPassReverbParams::GATE1A_DELAY, this, SLOT(onParamChange()));
	elements.gates[0].kPosGain->connectTo((int)AllPassReverbParams::GATE1A_POS_GAIN, this, SLOT(onParamChange()));
	elements.gates[0].kNegGain->connectTo((int)AllPassReverbParams::GATE1A_NEG_GAIN, this, SLOT(onParamChange()));

	elements.gates[1].kDelay->connectTo((int)AllPassReverbParams::GATE1B_DELAY, this, SLOT(onParamChange()));
	elements.gates[1].kPosGain->connectTo((int)AllPassReverbParams::GATE1B_POS_GAIN, this, SLOT(onParamChange()));
	elements.gates[1].kNegGain->connectTo((int)AllPassReverbParams::GATE1B_NEG_GAIN, this, SLOT(onParamChange()));

	elements.gates[2].kDelay->connectTo((int)AllPassReverbParams::GATE2A_DELAY, this, SLOT(onParamChange()));
	elements.gates[2].kPosGain->connectTo((int)AllPassReverbParams::GATE2A_POS_GAIN, this, SLOT(onParamChange()));
	elements.gates[2].kNegGain->connectTo((int)AllPassReverbParams::GATE2A_NEG_GAIN, this, SLOT(onParamChange()));

	elements.gates[3].kDelay->connectTo((int)AllPassReverbParams::GATE2B_DELAY, this, SLOT(onParamChange()));
	elements.gates[3].kPosGain->connectTo((int)AllPassReverbParams::GATE2B_POS_GAIN, this, SLOT(onParamChange()));
	elements.gates[3].kNegGain->connectTo((int)AllPassReverbParams::GATE2B_NEG_GAIN, this, SLOT(onParamChange()));

	btn = new QPushButton(this);
	connect(btn, SIGNAL(clicked()), this, SLOT(onToggleLCDmode()));

	analysis.lcd = new LCD(this);
	analysis.lcd->setCentered(true);
	analysis.needsUpdate = true;

}

Reverb1::~Reverb1() {

}

void Reverb1::onToggleLCDmode() {
	analysis.mode = (analysis.mode + 1) % 2;
	analysis.needsUpdate = true;
}


void Reverb1::refresh() {

	elements.gates[0].kDelay->setValueFromParam(getParameter((int)AllPassReverbParams::GATE1A_DELAY));
	elements.gates[0].kPosGain->setValueFromParam(getParameter((int)AllPassReverbParams::GATE1A_POS_GAIN));
	elements.gates[0].kNegGain->setValueFromParam(getParameter((int)AllPassReverbParams::GATE1A_NEG_GAIN));

	elements.gates[1].kDelay->setValueFromParam(getParameter((int)AllPassReverbParams::GATE1B_DELAY));
	elements.gates[1].kPosGain->setValueFromParam(getParameter((int)AllPassReverbParams::GATE1B_POS_GAIN));
	elements.gates[1].kNegGain->setValueFromParam(getParameter((int)AllPassReverbParams::GATE1B_NEG_GAIN));

	elements.gates[2].kDelay->setValueFromParam(getParameter((int)AllPassReverbParams::GATE2A_DELAY));
	elements.gates[2].kPosGain->setValueFromParam(getParameter((int)AllPassReverbParams::GATE2A_POS_GAIN));
	elements.gates[2].kNegGain->setValueFromParam(getParameter((int)AllPassReverbParams::GATE2A_NEG_GAIN));

	elements.gates[3].kDelay->setValueFromParam(getParameter((int)AllPassReverbParams::GATE2B_DELAY));
	elements.gates[3].kPosGain->setValueFromParam(getParameter((int)AllPassReverbParams::GATE2B_POS_GAIN));
	elements.gates[3].kNegGain->setValueFromParam(getParameter((int)AllPassReverbParams::GATE2B_NEG_GAIN));

	elements.kDryWet->setValueFromParam(getParameter((int)AllPassReverbParams::DRY_WET));

	if (analysis.needsUpdate) {
		showImpulseResponse();
		analysis.needsUpdate = false;
	}

}

void Reverb1::showImpulseResponse() {

	// fetch the impulse response
	this->getImpulseResponse(analysis.size, analysis.vec1.data());

	// create FFT?
	float* data = nullptr;
	if (analysis.mode == 1) {
		analysis.fft.getRealFFT(analysis.vec1.data(), analysis.vec2.data());
		data = analysis.vec2.data();
	} else {
		data = analysis.vec1.data();
	}

	// calculate LCD values
	unsigned int lcdVals = (unsigned int) analysis.vec3.size();
	for (unsigned int i = 0; i < lcdVals; ++i) {

		unsigned int j1 = (analysis.size-1) * (i+0) / lcdVals;
		unsigned int j2 = (analysis.size-1) * (i+1) / lcdVals;

		float v = 0;
		for (unsigned int j = j1; j < j2; ++j) {
			float vn = data[j];
			if ( std::abs(vn) > std::abs(v) ) {v = vn;}
		}

		analysis.vec3[i] = v;

	}

	// show the result
	analysis.lcd->setCentered(analysis.mode == 0);
	analysis.lcd->setValues(analysis.vec3);



}

void Reverb1::onParamChange() {

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

	// trigger LCD update on next gui refresh
	analysis.needsUpdate = true;

}


#include<QPainter>
void Reverb1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Skin::getImage("skin/reverb1.png", "PNG");
	p.drawImage(0,0,imgBg);

}

void Reverb1::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);

	for (unsigned int i = 0; i < 4; ++i) {
		int x = i * (96+8+4) + (8+2) + (i/2*8);
		elements.gates[i].kDelay->setGeometry(x+0,16, 0,0);
		elements.gates[i].kPosGain->setGeometry(x+32,16, 0,0);
		elements.gates[i].kNegGain->setGeometry(x+64,16, 0,0);
	}
	elements.kDryWet->setGeometry(680,16, 0,0);

	btn->setGeometry(430,0,16,16);

	connector->setGeometry(728+4,7, 0,0);

	//lcdImpResp->setGeometry(8,72,364,48);
	//lcdFreq->setGeometry(380,72,364,48);

	analysis.lcd->setGeometry(450,16,214,38);

}
