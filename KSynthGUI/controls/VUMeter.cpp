#include "VUMeter.h"
#include <QPaintEvent>
#include <QPainter>

#include <KSynth/misc/Units.h>
#include "../misc/Skin.h"

VUMeter::VUMeter(QWidget *parent) :
	QWidget(parent), value(0), lastMS(0) {

	setOrientation(VUMeterOrientation::VERTICAL);

}
#include <QDateTime>


/** set the VU value */
void VUMeter::setValue(float value) {

	// convert to DB
	//value = Units::amplitudeToDB(value);
	//if (value < -100) {value = -100;}
	//value = (value + 100) / 100.0f;

	// fallof depends on time between subsequenc calls
	uint64_t curMS =  QDateTime::currentMSecsSinceEpoch();
	float falloff = 1.5f * float(curMS - lastMS) / 1000.0f;
	lastMS = curMS;

	// upodate the value
	if (value > 1.0f)			{value = 1.0f;}
	if (this->value == value)	{return;}
	this->value -= falloff;
	if (value > this->value)	{this->value = value;}

	// refresh
	emit repaint();

}

/** get the VU value */
float VUMeter::getValue() const {
	return this->value;
}

void VUMeter::setOrientation(VUMeterOrientation orientation) {
	this->orientation = orientation;
	switch(this->orientation) {
		case VUMeterOrientation::VERTICAL:
			setMaximumSize(20,80);
			setMinimumSize(20,80);
			break;
		case VUMeterOrientation::HORIZONTAL:
			setMaximumSize(80,20);
			setMinimumSize(80,20);
			break;
	}
}

void VUMeter::paintEvent (QPaintEvent* event) {

	(void) event;

	static QImage imgBg = Skin::getImage("skin/vumeter_bg.png", "PNG");		// background
	static QImage imgBar = Skin::getImage("skin/vumeter_bar.png", "PNG");		// scaleable color-bar
	static QImage imgLED = Skin::getImage("skin/vumeter_leds.png", "PNG");		// led border

	QPainter p(this);

	// rotate?
	if (orientation == VUMeterOrientation::HORIZONTAL) {
		p.rotate(90);
		p.translate(0, -80);
	}

	// background
	p.drawImage(0,0, imgBg);

	// color-bar
	const int stepSize = 4;
	int h = int(std::ceil( float(imgBar.height()) * (0.99f - value) / float(stepSize))) * stepSize;
	p.drawImage(2,4+h, imgBar, 0,h, 20,imgBar.height()-h);

	// led-output
	p.drawImage(2,2, imgLED);

}
