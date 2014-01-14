#include "MasterTarget1.h"
#include <KSynth/analyzer/FFTAnalyzer.h>

#include "../controls/VUMeter.h"

#include "../model/Context.h"
#include "../controls/TextLabel.h"
#include "../controls/PinConnector.h"

#include "../misc/Helper.h"

MasterTarget1::MasterTarget1(Context& ctx, QWidget *parent) :
	RackElement(ctx, parent) {

	setSize(760, 48);

	elements.vuRight = new VUMeter(this);	elements.vuRight->setOrientation(VUMeterOrientation::HORIZONTAL);
	elements.vuLeft = new VUMeter(this);	elements.vuLeft->setOrientation(VUMeterOrientation::HORIZONTAL);
	lcd.setParent(this);

#ifdef WITH_FFTW3
	fft = new FFTAnalyzer(getSampleRate());
#endif


	elements.connector = new PinConnector(ctx, this, this);
	label->setVisible(false);

}

MasterTarget1::~MasterTarget1() {
#ifdef WITH_FFTW3
	delete fft;	fft = nullptr;
#endif
}

void MasterTarget1::process(Amplitude** inputs, Amplitude** outputs) {
	MasterTarget::process(inputs, outputs);
#ifdef WITH_FFTW3
	for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {fft->push(outputs[0][i]);}
#endif
}

void MasterTarget1::refresh() {

	elements.vuLeft->setValue(getVULeft());
	elements.vuRight->setValue(getVURight());

#ifdef WITH_FFTW3
	lcd.setValues(fft->get());
#endif

}


void MasterTarget1::onRackAttach() {
	// register as main sound target
	ctx.setMasterTarget(this);
}

void MasterTarget1::onRackDetach() {
	// unregister sound target
	ctx.setMasterTarget(nullptr);
}


//RackElement* MasterTarget1::createFromPrototype(Context& ctx) const {
//	throw "can't clone MasterTarget!";
//}

#include <QPainter>
void MasterTarget1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Helper::getSkinImage("skin/masterTarget.png", "PNG");
	p.drawImage(0,0, imgBg);

}

void MasterTarget1::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);
	elements.vuLeft->setGeometry(12,17, 0,0);
	elements.vuRight->setGeometry(108,17, 0,0);
	lcd.setGeometry(203+7,11, 0,0);
	elements.connector->setGeometry(728+4,12, 0,0);

}
