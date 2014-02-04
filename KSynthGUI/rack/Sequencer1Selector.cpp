#include "Sequencer1Selector.h"
#include "Sequencer1.h"


Sequencer1Selector::Sequencer1Selector(Sequencer1& seq, QWidget *parent) :
	QWidget(parent), seq(seq) {

	setMinimumSize(seq.mscaler.w+4, seq.mscaler.h+7);
	setMaximumSize(seq.mscaler.w+4, seq.mscaler.h+7);

	setMouseTracking(true);

}

#include <QMouseEvent>
void Sequencer1Selector::mouseReleaseEvent (QMouseEvent* e) {
	(void) e;
	mouse.down = false;
}
void Sequencer1Selector::mousePressEvent (QMouseEvent* e) {
	onMouse(e->x(), e->y());
	mouse.down = true;
}
#include <iostream>
void Sequencer1Selector::mouseMoveEvent(QMouseEvent* e) {
	if (mouse.down) {onMouse(e->x(), e->y());}
	int tick = 0;
	int curNoteRow = 0;
	seq.mscaler.getNoteTick(e->x(),e->y(), curNoteRow,tick);
	//std::cout << curNoteRow << std::endl;
	if (curNoteRow != mouse.curNoteRow) {
		mouse.curNoteRow = curNoteRow;
		emit repaint();
	}
}
void Sequencer1Selector::leaveEvent(QEvent* e) {
	(void) e;
	mouse.curNoteRow = -1;
	emit repaint();
}

void Sequencer1Selector::onMouse(int x, int y) {
	int noteNumber = 0;
	int tick = 0;
	seq.mscaler.getNoteTick(x,y, noteNumber,tick);
	if (tick < 0 || tick > (int) seq.getCurrentPattern().getTicks()) {return;}
	if (noteNumber == -1) {
		seq.getCurrentPattern().unbindNote( (unsigned int) tick );
	} else {
		seq.getCurrentPattern().bindNote( (unsigned int) tick, Note( (unsigned int) noteNumber ), 0.75);
	}

	emit repaint();
}

#include <QPainter>
#include <iostream>
void Sequencer1Selector::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QColor cBG(0,0,0);
	static const QColor cNoteRow(96,0,0);
	static const QColor cNote(220,0,0);
	static QPen pGridA(QColor(128,128,128), 4);
	static QPen pGridB(QColor(128,128,128), 1);

	p.setRenderHint(QPainter::Antialiasing, false);

	// background
	p.setBrush(cBG);
	p.drawRect(0,0,this->width()-1,this->height()-1);

	const int noteH = 8;

	// draw h-indicator
	p.setPen(Qt::NoPen);
	p.setBrush(cNoteRow);
	{
		int y1,h;
		seq.mscaler.getNoteY(mouse.curNoteRow - seq.mscaler.oct*12, y1, h);
		p.drawRect(0,y1, seq.mscaler.w,seq.mscaler.noteH);
	}

	// draw notes
	p.setPen(Qt::NoPen);
	p.setBrush(cNote);
	PatternSequencerPattern& pattern = seq.getCurrentPattern();
	const std::vector<PatternSequencerPatternEntry>& entries = pattern.getEntries();
	for (unsigned int i = 0; i < entries.size(); ++i) {

		// only draw set notes
		if (!entries[i].used)					{continue;}

		// get the position
		int x; int y; int w; int h;
		seq.mscaler.getBeatX( int(i), x, w);
		seq.mscaler.getNoteY(entries[i].note, y, h);

		// ignore lower/higher octaves
		if (y > seq.mscaler.y + seq.mscaler.h)	{continue;}
		if (y < seq.mscaler.y)					{continue;}

		// draw
		p.drawRect(x+1,y, w-1,h+1);

	}

	// grid hor
	p.setPen(pGridB);
	for (int i = 0; i <= 12; ++i) {
		int oy = i * noteH;
		p.drawLine(seq.mscaler.x, seq.mscaler.y+oy, seq.mscaler.x+seq.mscaler.w, seq.mscaler.y+oy);
	}

	// grid ver
	for (unsigned int i = 0; i <= seq.getCurrentPattern().getTicks(); ++i) {
		int x; int w;
		int y = seq.mscaler.y;
		int h = seq.mscaler.h;
		unsigned int granularity = seq.getCurrentPattern().getGranularity();
		seq.mscaler.getBeatX( int(i), x, w);
		if ( (i % granularity) == 0 ) {p.setPen(pGridA); y+=2; h-=2;} else {p.setPen(pGridB);}
		p.drawLine(x, y, x, y + h);
	}

}
