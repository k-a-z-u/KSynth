#include "EditorHelper.h"

#include <QPainter>
#include <QLinearGradient>

void EditorHelper::drawBorder3D(QPainter& p, int x, int y, int w, int h) {

	static QColor w1(255,255,255,128);
	static QColor w2(255,255,255,192);

	static QColor b1(0,0,0,128);
	static QColor b2(0,0,0,164);

	--w;
	--h;

	QLinearGradient gVer (0, 0, w, 0);
	QLinearGradient gHor (0, 0, 0, h);
	QPen pen;

	// bright
	gVer.setColorAt(0, w2); gVer.setColorAt(1, w1); pen.setBrush(gVer); p.setPen(pen);
	p.drawLine(x, y, x+w, y);
	gHor.setColorAt(0, w2); gHor.setColorAt(1, w1); pen.setBrush(gHor); p.setPen(pen);
	p.drawLine(x, y, x, y+h);

	// dark
	gVer.setColorAt(0, b1); gVer.setColorAt(1, b2); pen.setBrush(gVer); p.setPen(pen);
	p.drawLine(x, y+h, x+w, y+h);
	gHor.setColorAt(0, b1); gHor.setColorAt(1, b2); pen.setBrush(gHor); p.setPen(pen);
	p.drawLine(x+w, y, x+w, y+h);

}


