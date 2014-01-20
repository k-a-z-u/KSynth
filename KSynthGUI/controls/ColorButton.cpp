#include "ColorButton.h"
#include "../misc/Skin.h"

ColorButton::ColorButton(QWidget *parent) :
	MidiUI(parent), bgColor(0,0,0), isDown(false) {

	setMinimumSize(28, 28);
	setMaximumSize(28, 28);

}

void ColorButton::setColor(QColor c) {
	bool changed = bgColor != c;
	this->bgColor = c;
	if (changed) {emit repaint();}
}


void ColorButton::mouseReleaseEvent (QMouseEvent* e) {
	Q_UNUSED(e);
	this->isDown = false;
	emit repaint();
	emit onChange();
}

void ColorButton::mousePressEvent (QMouseEvent* e) {
	Q_UNUSED(e);
	this->isDown = true;
	emit repaint();
}

#include <QPainter>
void ColorButton::paintEvent (QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Skin::getImage("skin/colorbutton1_bg.png", "PNG");
	static QImage imgUp = Skin::getImage("skin/colorbutton1_up", "PNG");
	static QImage imgDown = Skin::getImage("skin/colorbutton1_down", "PNG");

	p.setBrush(bgColor);
	p.setPen(Qt::NoPen);

	p.drawImage(0,0,imgBg);
	p.drawRect(3,3,28-6,28-6);

	if (isDown) {
		p.drawImage(0,0,imgDown);
	} else {
		p.drawImage(0,0,imgUp);
	}

}

ParamValue ColorButton::getValueAsParam() const {
	return 0.0f;
}

void ColorButton::setValueFromParam(ParamValue val) {
	Q_UNUSED(val);
}
