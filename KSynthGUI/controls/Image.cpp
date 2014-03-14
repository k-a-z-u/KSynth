#include "Image.h"
#include <QPainter>

Image::Image(QWidget *parent) :
	QWidget(parent) {
	;
}

void Image::setImage(const QImage& img) {
	this->img = img;
	resize(img.width(), img.height());
	emit repaint();
}

void Image::paintEvent(QPaintEvent* e) {
	Q_UNUSED(e);
	QPainter p(this);
	p.drawImage(0,0,img);
}
