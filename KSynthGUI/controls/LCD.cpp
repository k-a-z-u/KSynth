#include "LCD.h"

#include <QPainter>

LCD::LCD(QWidget *parent) : QWidget(parent), centered(false) {

	//setSize(138, 26);

	paint.dashes << 0.5f << 0.5f;
	//paint.pen.setStyle(Qt::DotLine);
	paint.pen.setColor(QColor(255,255,255,180));
	paint.pen.setDashPattern(paint.dashes);
	paint.pen.setWidth(2);

}

void LCD::setSize(int w, int h) {
	setMinimumSize(w, h);
	setMaximumSize(w, h);
}

void LCD::setCentered(bool centered) {
	this->centered = centered;
}

bool LCD::isCentered() const {
	return this->centered;
}

void LCD::setValues( const std::vector<float>& next ) {

	// store new values
	this->values = next;

	// trigger repaint
	emit repaint();

}

void LCD::updateBG() {

	int w = width();
	int h = height();

	if (bg.width() == w && bg.height() == h) {return;}
	bg = QImage(w, h, QImage::Format_ARGB32);
	bg.fill(0x00000000);

	QPainter p(&bg);
	QPen pen = paint.pen;
	pen.setColor(QColor(255,255,255,16));
	p.setPen(pen);

	for (int x = 1; x < width(); x += 3) {
		p.drawLine(x,0,x,height());
	}

	p.end();
}

void LCD::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);

	// nothing to draw?
	if (values.empty()) {return;}

	QPainter p(this);

	// update and draw the background ground grid
	updateBG();
	p.drawImage(0,0,bg);


	p.setPen(paint.pen);
	float h = float(height());

	// draw all bars
	for (unsigned int i = 0; i < values.size(); ++i) {

		unsigned int x = 2 + i*3;
		if ((int)x > width()) {break;}

		if (!centered) {
			int y1 = int(h - values[i] * h);
			p.drawLine(x-1, int(h), x-1, y1);

		} else if (centered == 1) {
			int y1 = int(h/2 - values[i] * h * 0.45);
			p.drawLine(x-1, int(h/2), x-1, y1);

		}

	}

}
