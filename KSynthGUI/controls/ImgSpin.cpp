#include "ImgSpin.h"

#include <QPushButton>
#include <QPainter>
#include "../misc/Skin.h"

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

	static QImage imgBg = Skin::getImage("skin/imgspin1.png", "PNG");
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

#include <QWheelEvent>
void ImgSpin::wheelEvent(QWheelEvent *event) {
	int steps = event->delta() / 120;
	if (steps < 0) {emit onPrev();}
	if (steps > 0) {emit onNext();}
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
	return ParamValue(0u, (unsigned int) entries.size() - 1, curIdx);
}

void ImgSpin::setValueFromParam(ParamValue val) {
	setSelectedIndex(val.asUInt(0u, (unsigned int) entries.size() - 1));
}
