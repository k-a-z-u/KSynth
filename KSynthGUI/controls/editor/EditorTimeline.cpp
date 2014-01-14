#include "EditorTimeline.h"

EditorTimeline::EditorTimeline(Editor& editor, QWidget *parent) :
	QWidget(parent), editor(editor) {

	setMinimumHeight(28);
	setMaximumHeight(28);

}


#include <QPainter>
void EditorTimeline::paintEvent(QPaintEvent* e) {

	//QWidget::paintEvent(e);
	Q_UNUSED(e);

	static QImage img = QImage(0,0, QImage::Format_ARGB32);

	// caching
	if (img.width() != width() || img.height() != height()) {

		// create new image
		img = QImage(width(), height(), QImage::Format_ARGB32);
		img.fill(0x00000000);
		QPainter p(&img);

		QColor cFill(230,230,230);

		// sizing temporals
		const unsigned int w = width() - 1;
		const unsigned int h = height() - 1;

		// convert 128th notes to the requested base (= zoom)
		const unsigned int len = editor.getScaler().getNumBars(editor.getSongLength());

		const unsigned int sx = editor.getScaler().getSX();
		const unsigned int nw = editor.getScaler().getNW();

		const unsigned int bw = editor.getScaler().getBarWidth();

		// background gradient
		QLinearGradient grad(0, 0, 0, height());
		grad.setColorAt(0, cFill);
		grad.setColorAt(1, cFill.darker(140));
		p.setPen(Qt::NoPen);
		p.setBrush(grad);
		p.drawRect(0,0,width(),height());

		// draw bars (one full beat) and subs
		p.setPen(QColor(96,96,96));
		for (unsigned int bar = 0; bar < len; ++bar) {

			int x = sx + bw*bar;
			p.drawLine(x, 0, x, height());
			p.drawText(x+2, 10, std::to_string(bar).c_str());

			// draw sub lines
			for (unsigned int sub = 1; sub < 8; ++sub) {
				int xx = x+(nw/2)*sub;
				int sy = (int) ( (sub % 2) ? (height()*0.8) : (height()*0.7) );
				p.drawLine(xx, sy, xx, height());
			}

		}

		// outline
		// draw dark outline
		p.setPen(cFill.darker(200));
		p.drawLine(0,h,w,h);
		p.drawLine(w,0,w,h);

		// draw bright outline
		p.setPen(cFill.lighter());
		p.drawLine(0,0,w,0);
		p.drawLine(0,0,0,h);


	}

	// update
	QPainter p(this);
	p.drawImage(0,0,img);

}
