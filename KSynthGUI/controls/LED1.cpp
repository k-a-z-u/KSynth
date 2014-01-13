#include "LED1.h"
#include "../misc/Helper.h"

LED1::LED1(QWidget *parent) :
	QWidget(parent), on(false) {

	setMinimumSize(13, 13);
	setMaximumSize(13, 13);

}

void LED1::setValue(bool on) {
	bool changed = on != this->on;
	this->on = on;
	if (changed) {emit repaint();}
}

#include <QPainter>
void LED1::paintEvent (QPaintEvent* event) {

	(void) event;

	static QImage imgOff = Helper::getSkinImage("skin/led1_off.png", "PNG");
	static QImage imgOn = Helper::getSkinImage("skin/led1_on.png", "PNG");

	QPainter p(this);
	if (on) {
		p.drawImage(0,0,imgOn);
	} else {
		p.drawImage(0,0,imgOff);
	}

}
