#include "ImgSpin.h"

#include <QPushButton>
#include <QPainter>
#include "../misc/Helper.h"

ImgSpin::ImgSpin(QWidget *parent) :
	MidiUI(parent), curIdx(0) {

	btnNext = new QPushButton();
	btnPrev = new QPushButton();
	btnNext->setParent(this);
	btnPrev->setParent(this);
	connect(btnNext, SIGNAL(clicked()), this, SLOT(onNext()));
	connect(btnPrev, SIGNAL(clicked()), this, SLOT(onPrev()));

}

void ImgSpin::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);

	const int btnH = (this->height()-0) / 2;
	const int btnW = 14;//this->width()/3;
	btnNext->setGeometry(this->width()-btnW-1,0,		btnW,btnH);
	btnPrev->setGeometry(this->width()-btnW-1,btnH-1,	btnW,btnH);

}

void ImgSpin::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Helper::getSkinImage("skin/imgspin1.png", "PNG");
	p.drawImage(0,0, imgBg);

	if (!entries.empty()) {
		p.drawImage(2,2, entries[curIdx].img);
	}

}

void ImgSpin::add(const QImage& img, int value) {

	entries.push_back(Entry(img, value));

	setMinimumSize(img.width()+2+14, img.height()+2);
	setMaximumSize(img.width()+2+14, img.height()+2);

}

void ImgSpin::onNext() {
	if (entries.size() <= curIdx +1) {return;}
	++curIdx;
	emit repaint();
	emit onChange();
}

void ImgSpin::onPrev() {
	if (curIdx == 0) {return;}
	--curIdx;
	emit repaint();
	emit onChange();
}

unsigned int ImgSpin::getSelectedIndex() {
	return curIdx;
}

void ImgSpin::setSelectedIndex(unsigned int idx) {
	if (idx >= entries.size()) {return;}
	bool changed = idx != getSelectedIndex();
	this->curIdx = idx;
	if (changed) {emit repaint();}
}

int ImgSpin::getSelectedValue() {
	return entries[curIdx].value;
}

ParamValue ImgSpin::getValueAsParam() const {
	return ParamValue(0, (unsigned int) entries.size() - 1, curIdx);
}

void ImgSpin::setValueFromParam(ParamValue val) {
	setSelectedIndex(val.asInt(0, (unsigned int) entries.size() - 1));
}