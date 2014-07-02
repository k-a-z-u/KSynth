#include "Reverb1LCD.h"

#include <QPainter>

Reverb1LCD::Reverb1LCD(QWidget* parent) : QWidget(parent) {

}

void Reverb1LCD::paintEvent(QPaintEvent* event) {

	(void) event;
	resizeImageBuffer();

	QPainter qp(this);
	qp.drawImage(0,0,img);

}

void Reverb1LCD::drawImpulseResponse(const float* data, unsigned int size) {

	resizeImageBuffer();
	img.fill(0xFF000000);

	QPainter qp;
	qp.begin(&img);
	qp.setPen(QColor(255,255,255));

	int w = img.width();
	int h2 = img.height() / 2;

	for (int i = 0; i < w; ++i) {
		int idx1 = size * (i+0) / w;
		int idx2 = size * (i+1) / w;
		float val = 0;
		for (int j = idx1; j < idx2; ++j) {
			float t = data[j];
			if (std::abs(t) > std::abs(val)) {val = t;}
		}
		int y = int( float(h2) - float(h2) * val );
		qp.drawLine(i, h2, i, y);
	}

	qp.end();
	emit repaint();

}

void Reverb1LCD::drawValues(const float* data, unsigned int size) {

	resizeImageBuffer();
	img.fill(0xFF000000);

	QPainter qp;
	qp.begin(&img);
	qp.setPen(QColor(255,255,255));

	int w = img.width();
	int h = img.height();

	for (int i = 0; i < w; ++i) {
		int idx = size * (i+0) / w;
		int y = int( float(h) - float(h) * data[idx] );
		qp.drawLine(i, h, i, y);
	}

	qp.end();
	emit repaint();

}

void Reverb1LCD::resizeImageBuffer() {
	if (img.width() == this->width() && img.height() == this->height()) {return;}
	img = QImage(this->width(), this->height(), QImage::Format_ARGB32);
	img.fill(0xFF000000);
}
