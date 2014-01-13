#include "Slider1.h"

#include <QPainter>
#include "../misc/Helper.h"

Slider1::Slider1(QWidget *parent) :
	MidiUI(parent) {

	value.value = 0;
	value.min = 0;
	value.max = 127;

	size.uBorder = 6;
	size.lBorder = 16;
	size.way = 66;
	size.w = 28;
	size.h = size.way + size.uBorder + size.lBorder;
	size.sldTop = 10;

	setMinimumSize(size.w,size.h);
	setMaximumSize(size.w,size.h);

}


Slider1::Slider1(const std::string& title, int min, int max, int value, QWidget *parent) :
	Slider1(parent) {
	setTitle(title);
	this->value.min = min;
	this->value.max = max;
	this->value.value = value;
}

void Slider1::addSnap(int val, int size) {
	snapper.add(val, size);
}

void Slider1::setTitle(const std::string &title){
	this->title = title;
	emit repaint();
}

void Slider1::setValue(int v) {

	int oldValue = value.value;
	if		(v < value.min)	{value.value = value.min;}
	else if	(v > value.max)	{value.value = value.max;}
	else					{value.value = v;}
	int newValue = value.value;

	// repaint only if value changed
	if (oldValue != newValue) {emit repaint();}

}

int Slider1::getValue() {
	return value.value;
}

ParamValue Slider1::getValueAsParam() const {
	return ParamValue(value.min, value.max, value.value);
}

void Slider1::setValueFromParam(ParamValue val) {
	setValue(val.asInt(value.min, value.max));
}

#include <iostream>
#include <QMouseEvent>
void Slider1::mouseReleaseEvent (QMouseEvent* e) {
	Q_UNUSED(e);
	//mouseDown.isDown = false;
}
void Slider1::mousePressEvent (QMouseEvent* e) {
	//mouseDown.isDown = true;
	//mouseDown.x = e->x();
	//mouseDown.y = e->y();
	recalc(e->x(), e->y());
}
void Slider1::mouseMoveEvent(QMouseEvent* e) {
	recalc(e->x(), e->y());
}

#include <QToolTip>
void Slider1::recalc(int x, int y) {

	Q_UNUSED(x);
	y -= size.sldTop;// - mouseDown.y;
	int newValue = value.max - int( float(value.max-value.min) * float(y) / float(size.way) );
	newValue = snapper.getSnapped(newValue);

	// set the new value
	int oldValue = getValue();
	setValue(newValue);
	newValue = getValue();

	// value changed?
	if (oldValue != newValue) {

		// show tooltip
		QToolTip::showText( this->mapToGlobal( QPoint( size.w, y-size.w/2 ) ), QString::number(getValue()));

		// inform listeners
		emit onChange();

	}

}


void Slider1::paintEvent (QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Helper::getSkinImage("skin/slider1_bg.png", "PNG");
	static QImage imgMain = Helper::getSkinImage("skin/slider1_main.png", "PNG");
	static QColor cText(255,255,255);
	static QFont font; font.setPixelSize(7); font.setBold(true);

	// text
	p.setPen(cText);
	p.setFont(font);
	//p.drawText(0,2, size.w,10, Qt::AlignCenter, title.c_str());
	p.drawText(0,size.h-34, size.w,size.h-28, Qt::AlignCenter, title.c_str());

	// background
	p.drawImage(0,size.uBorder, imgBg);

	// slider
	int y = size.way - int( float(value.value - value.min) * float(size.way) / float(value.max - value.min) );
	p.drawImage(0,y, imgMain);




}
