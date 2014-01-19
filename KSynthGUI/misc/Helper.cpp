#include "Helper.h"

#include <QPainter>
#include <KLib/fs/File.h>

QImage Helper::getSkinImage(const std::string& file, const std::string& type) {

	K::File f(file);
	std::cout << f.getAbsolutePath() << std::endl;
	return QImage(f.getAbsolutePath().c_str(), type.c_str());

}

QIcon Helper::getSkinIcon(const std::string& file, const std::string& type) {
	QImage img = getSkinImage(file, type);
	return QIcon( QPixmap::fromImage(img) );
}

QCursor Helper::getSkinCursor(const std::string& file, const std::string& type, int hotX, int hotY) {
	QImage img = getSkinImage(file, type);
	return QCursor( QPixmap::fromImage(img), hotX, hotY );
}

QImage Helper::getForOscFunction(SimpleOscillator2Mode mode, unsigned int w, unsigned int h) {

	SimpleOscillator2 osc;
	//osc.setFrequency(1);
	osc.setMode(mode);

	QImage img(w,h, QImage::Format_ARGB32);
	QPainter p;
	p.begin(&img);
	p.setRenderHint(QPainter::Antialiasing, true);

	img.fill(0x00000000);
	//	p.setBrush(QColor(0,0,0,0));
	//	p.drawRect(0,0,w,h);

	QPen pen(QColor(255,0,0));
	pen.setWidthF(2.1f);
	p.setPen(pen);

	// drawing height and step size
	const int dh = h-6;
	const int step = 1;

	for (unsigned int x = 0; x < w-1; x += step) {
		float a1 = osc.get((float)(x+0   ) / (float) w);
		float a2 = osc.get((float)(x+step) / (float) w);
		int y1 = int( float(h/2) + a1 * float(dh/2) );
		int y2 = int( float(h/2) + a2 * float(dh/2) );
		p.drawLine(x,y1,x+1,y2);
	}



	//	// remove completely
	//	for (unsigned int x = 0; x < w; ++x) {
	//		for (unsigned int y = 0; y < h; ++y) {
	//			if (x%3==0 || y%3==0) {img.setPixel(x,y,QColor(0,0,0,64).rgba());}
	//		}
	//	}

	p.end();

	LEDify(img);

	return img;

}

QImage Helper::getForNumber(int number, unsigned int w, unsigned int h) {

	// the image
	QImage img(w,h, QImage::Format_ARGB32);
	img.fill(0x00000000);

	// painter
	static QPainter p;
	p.begin(&img);
	p.setRenderHint(QPainter::Antialiasing, true);

	// font
	static QFont font; font.setPixelSize(h); font.setBold(true);
	p.setFont(font);

	// color
	static QColor cTxt(255,0,0);
	p.setPen(cTxt);

	p.drawText(0, h*7/8, QString::number(number));

	p.end();

	LEDify(img);

	return img;

}

void Helper::LEDify(QImage& img) {

	// create a copy
	QImage img2 = img;



	// painter
	static QPainter p;
	p.begin(&img);



	unsigned int w = img.width();
	unsigned int h = img.height();


	// clear original
	img.fill(0x00000000);

	static QBrush brush;
	brush.setColor(QColor(255,0,0,255));
	p.setPen(Qt::NoPen);
	p.setBrush(QColor(255,0,0));

	// fill rects
	for (unsigned int x = 0; x < w; x += 2) {
		for (unsigned int y = 0; y < h; y += 2) {

			unsigned int r = 0;
			unsigned int g = 0;
			unsigned int b = 0;
			unsigned int a = 0;
			unsigned int cnt = 4;
			for (int x1 = 0; x1 <= 1; ++x1) {
				for (int y1 = 0; y1 <= 1; ++y1) {
					QRgb rgba = img2.pixel(x+x1, y+y1);
					r += qRed(rgba);
					g += qGreen(rgba);
					b += qBlue(rgba);
					a += qAlpha(rgba);
				}
			}

			p.setBrush(QColor(r/cnt, g/cnt, b/cnt, a/cnt));
			p.drawRect(x,y, 2,2);

		}
	}

	// draw
	static QPen pen;
	pen.setColor(QColor(0,0,0,48));
	pen.setWidth(1);
	p.setPen(pen);

//	// draw vertical grid
	for (unsigned int x = 0; x < w; x += 2) {p.drawLine(x,0,x,h);}

//	// draw horizontal grid
	for (unsigned int y = 0; y < h; y += 2) {p.drawLine(0,y,w,y);}

	p.end();

}
