#include "Reverb1.h"

#include <KSynth/fx/Reverb.h>
#include "../controls/Knob.h"
#include "../misc/Helper.h"

Reverb1::Reverb1(Context& ctx, QWidget *parent) :
	RackElement(ctx, parent), Reverb() {

	setMinimumSize(256,48);
	setMaximumSize(256,48);

	elements.kDelay = new Knob("delay", 0, 100, 30);
	elements.kDry = new Knob("dry", 0, 100, 50);
	elements.kNum = new Knob("num", 0, 16, 4);

	elements.kDelay->setParent(this);
	elements.kDry->setParent(this);
	elements.kNum->setParent(this);

	connect(elements.kDelay, SIGNAL(onChange()), this, SLOT(onValueChanged()));
	connect(elements.kDry, SIGNAL(onChange()), this, SLOT(onValueChanged()));
	connect(elements.kNum, SIGNAL(onChange()), this, SLOT(onValueChanged()));

}

Reverb1::~Reverb1() {

}

//RackElement* Reverb1::createFromPrototype(Context& ctx) const {
//	return new Reverb1( (QWidget*) parent() );
//}

//std::string Reverb1::getDeviceType() const {
//	return "Reverb1";
//}

void Reverb1::refresh() {

}

void Reverb1::onValueChanged() {
	config(elements.kNum->getValue(), float(elements.kDelay->getValue()) / 100.0f, float(elements.kDry->getValue()) / 100.0f);
	emit repaint();
}

#include<QPainter>
void Reverb1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Helper::getSkinImage("skin/reverb1.png", "PNG");
	p.drawImage(0,0,imgBg);

}

void Reverb1::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);

	elements.kDry->setGeometry(10,4, 0,0);
	elements.kNum->setGeometry(60,4, 0,0);
	elements.kDelay->setGeometry(110,4, 0,0);

}
