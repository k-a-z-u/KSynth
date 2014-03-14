#include "ColorButton.h"
#include "../misc/Skin.h"

ColorButton::ColorButton(QWidget *parent) :
	MidiUI(parent), bgColor(0,0,0), isDown(false), big(false) {

	setBig(false);

}

void ColorButton::setColor(QColor c) {
	bool changed = bgColor != c;
	this->bgColor = c;
	if (changed) {emit repaint();}
}

void ColorButton::setBig(bool big) {
	this->big = big;
	int s = big ? 28*2 : 28;
	setMinimumSize(s, s);
	setMaximumSize(s, s);
}

void ColorButton::mouseReleaseEvent (QMouseEvent* e) {
	Q_UNUSED(e);
	this->isDown = false;
	emit repaint();
	emit onChange();
}

QMouseEvent* ColorButton::getLastEvent() {
	return lastEvt;
}

#include <QMouseEvent>
void ColorButton::mousePressEvent (QMouseEvent* e) {
	this->isDown = e->button() == Qt::LeftButton;
	this->lastEvt = e;
	emit repaint();
}

#include <QPainter>
void ColorButton::paintEvent (QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage img1Bg = Skin::getImage("skin/colorbutton1_bg.png", "PNG");
	static QImage img1Up = Skin::getImage("skin/colorbutton1_up", "PNG");
	static QImage img1Down = Skin::getImage("skin/colorbutton1_down", "PNG");

	static QImage img2Bg = Skin::getImage("skin/colorbutton2_bg.png", "PNG");
	static QImage img2Up = Skin::getImage("skin/colorbutton2_up", "PNG");
	static QImage img2Down = Skin::getImage("skin/colorbutton2_down", "PNG");

	p.setBrush(bgColor);
	p.setPen(Qt::NoPen);

	p.drawImage(0,0, big ? img2Bg : img1Bg);

	if (big) {
		p.drawRect(6,6,56-12,56-12);
	} else {
		p.drawRect(3,3,28-6,28-6);
	}

	if (isDown) {
		p.drawImage(0,0, big ? img2Down : img1Down);
	} else {
		p.drawImage(0,0, big ? img2Up : img1Up);
	}

}

ParamValue ColorButton::getValueAsParam() const {
	return 0.0f;
}

void ColorButton::setValueFromParam(ParamValue val) {
	Q_UNUSED(val);
}
