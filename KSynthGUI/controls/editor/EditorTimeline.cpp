#include "EditorTimeline.h"
#include "EditorSlider.h"
#include "../../model/Context.h"
#include <KSynth/Sequencer.h>

EditorTimeline::EditorTimeline(Editor& editor, QWidget *parent) :
	QWidget(parent), editor(editor), lastNW4(0) {

	setMinimumHeight(28);
	setMaximumHeight(28);

}

void EditorTimeline::mousePressEvent(QMouseEvent* e) {
	//int x = editor.getScaler().getNoteDelay()
	editor.getSlider()->move(e->x(), 0);
	TimeBase128 time = editor.getScaler().getTime128(e->x());
	editor.getContext().getSequencer()->jumpTo(time);
}

#include <QPainter>
void EditorTimeline::paintEvent(QPaintEvent* e) {

	//QWidget::paintEvent(e);
	Q_UNUSED(e);

	QPainter p(this);

	static QImage imgTile;

	// sizing temporals
	const unsigned int w = width() - 1;
	const unsigned int h = height() - 1;
	const unsigned int sx = editor.getScaler().getSX();
	//const unsigned int nw = editor.getScaler().getNW();
	unsigned int bw = editor.getScaler().getBarWidth();

	static const QColor cTxt(96,96,96);
	static const QColor cLines(96,96,96);
	static const QColor cFill(230,230,230);

	const int steps = (bw >= 32) ? (1) : (2);
	bw*=steps;

	// as background and ticks provide a repeating pattern, we cache one tile
	// (width of one bar) and draw several tiles side by side

	if (lastNW4 != editor.getScaler().get4thWidth()) {
		lastNW4 = editor.getScaler().get4thWidth();

		// create new image
		imgTile = QImage(bw, height(), QImage::Format_ARGB32);
		imgTile.fill(0x00000000);
		QPainter p(&imgTile);

		// background gradient
		QLinearGradient grad(0, 0, 0, height());
		grad.setColorAt(0, cFill);
		grad.setColorAt(1, cFill.darker(140));
		p.setPen(Qt::NoPen);
		p.setBrush(grad);
		p.drawRect(0,0, bw,height());

		// lines
		p.setPen( cLines );
		const int x = sx;
		p.drawLine(x, 0, x, h);

		// 4th sub lines
		if (bw >= 32) {
			for (unsigned int sub = 1; sub < 4; ++sub) {
				const int xx = x+bw*sub/4;
				const int sy = int(h*0.7);
				p.drawLine(xx, sy, xx, h);
			}
		}

		// 8th sub lines
		if (bw >= 64) {
			for (unsigned int sub = 0; sub < 4; ++sub) {
				const int xx = x+bw*sub/4 + bw/8;
				const int sy = int(h*0.8);
				p.drawLine(xx, sy, xx, h);
			}
		}

	}

	bw /= steps;

	// convert 128th notes to the requested base (= zoom)
	const unsigned int len = editor.getScaler().getNumBars(editor.getSongLength());

	// draw visible background tiles
	for (unsigned int x = 0; x < (unsigned int) w; x += bw*steps) {

		// tile visible?
		QRect tileRect(x,0, imgTile.width(), imgTile.height());
		if (!e->region().contains(tileRect)) {continue;}

		// draw
		p.drawImage(x,0,imgTile);

	}

	// draw text
	p.setPen( cTxt );
	for (unsigned int bar = 0; bar < len; bar += steps) {

		const int x = sx + bw*bar + 2;
		const int y = 12;

		// text visible?
		if (!e->region().contains(QPoint(x,y))) {continue;}

		// draw
		p.drawText(x+2, y, std::to_string(bar).c_str());

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
