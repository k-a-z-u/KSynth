#include "Knob.h"

#include <QPaintEvent>
#include <QPainter>
#include "../misc/Helper.h"

#define KNOB_S	31

Knob::Knob(QWidget* parent) :
	MidiUI(parent) {

	setMinimumSize(KNOB_S, KNOB_S+4);
	setMaximumSize(KNOB_S, KNOB_S+4);

	value.value = 0;
	value.min = 0;
	value.max = 127;

	title = "misc";

}

Knob::Knob(const std::string& title, int min, int max, int val, QWidget* parent) :
	Knob(parent) {
	setTitle(title);
	value.min = min;
	value.max = max;
	value.value = val;
}

ParamValue Knob::getValueAsParam() const {
	//return (value.value - value.min) / (float) (value.max - value.min);
	return ParamValue(value.min, value.max, value.value);
}

void Knob::setValueFromParam(ParamValue val) {
	//setValue(value.min + (value.max-value.min) * val);
	setValue(val.asInt(value.min, value.max));
}

int Knob::getValue() const {return value.value;}

void Knob::setValue(int v) {

	int oldValue = value.value;
	if (v > value.max) {v = value.max;}
	if (v < value.min) {v = value.min;}
	value.value = v;
	int newValue = value.value;

	// value changed? -> repaint
	if (oldValue != newValue) {emit repaint();}

}


#include <iostream>
void Knob::mouseReleaseEvent (QMouseEvent* e) {
	Q_UNUSED(e);
	mouseState.isDown = false;
}
void Knob::mousePressEvent (QMouseEvent* e) {
	mouseState.downValue = getValue();
	mouseState.isDown = true;
	mouseState.x = e->x();
	mouseState.y = e->y();
}

#include <QToolTip>
void Knob::mouseMoveEvent(QMouseEvent* e) {

	// 100 px = one complete rotation
	int pixel = mouseState.y - e->y();
	int steps = pixel * (value.max - value.min) / 100;

	// calculate new value and perform snapping
	int nextVal = mouseState.downValue + steps;
	nextVal = snapper.getSnapped(nextVal);

	// apply
	int oldValue = getValue();
	setValue(nextVal);
	int newValue = getValue();

	// value changed?
	if (oldValue != newValue) {

		// show tooltip
		QToolTip::showText( this->mapToGlobal( QPoint(32,0) ), QString::number(getValue()), this);

		emit onChange();

	}


}

void Knob::setTitle(const std::string &str) {
	this->title = str;
}

void Knob::addSnap(int val, int size) {
	snapper.add(val, size);
}

void Knob::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Helper::getSkinImage("skin/knob1_bg.png", "PNG");
	static QImage imgMain = Helper::getSkinImage("skin/knob1_main.png", "PNG");
	static QImage imgTop = Helper::getSkinImage("skin/knob1_top.png", "PNG");
	static QColor cText(255,255,255);
	static QFont font; font.setPixelSize(7); font.setBold(true);

	// get the knob's rotation
	float rot = (float) (value.value - value.min) / (float) (value.max - value.min) * 240.0f;

	// title
	p.setPen(cText);
	p.setFont(font);
	p.drawText(0,20, KNOB_S,KNOB_S-7, Qt::AlignCenter, title.c_str());

	// shadow
	p.drawImage(0,0, imgBg);

	// knob
	p.translate(KNOB_S/2.0f, KNOB_S/2.0f);			// center of the knob's image
	p.rotate(rot);
	p.translate(-KNOB_S/2.0f, -KNOB_S/2.0f);
	p.setRenderHint(QPainter::SmoothPixmapTransform, true);
	p.drawImage(0,0, imgMain);
	p.setRenderHint(QPainter::SmoothPixmapTransform, false);

	// light
	p.resetMatrix();
	p.drawImage(0,0, imgTop);

}