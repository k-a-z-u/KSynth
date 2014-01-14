#include "EditorSheetNote.h"

#include "EditorSheet.h"
#include "Editor.h"


EditorSheetNote::EditorSheetNote(EditorSheet& sheet, EditorNote note, QWidget *parent) :
	QWidget(parent), sheet(sheet), note(note), selected(false), cached(false) {

	setFocusPolicy(Qt::ClickFocus);

	updateSize();

}

#include <iostream>
void EditorSheetNote::updateSize() {

	unsigned int x = sheet.getEditor().getScaler().getObjectWidth(note.on->getDelay());
	unsigned int y = (ES_MAX_NOTE - note.on->d1) * sheet.getEditor().getScaler().getNH();
	unsigned int h = sheet.getEditor().getScaler().getNH();
	unsigned int w = sheet.getEditor().getScaler().getNoteWidth(note);

	setGeometry(x+1, y+1, w-1, h-1);
//	setFixedSize(w-1, h-1);
//	move(x+1,y+1);

}

#include <QFocusEvent>
void EditorSheetNote::focusInEvent(QFocusEvent* e) {
	sheet.setSelected(this);
}

void EditorSheetNote::focusOutEvent(QFocusEvent*) {

}

void EditorSheetNote::setSelected(bool selected) {
	bool changed = this->selected != selected;
	this->selected = selected;
	if (changed) {cached = false; emit repaint();}
}

#include <QPainter>
void EditorSheetNote::paintEvent(QPaintEvent* e) {

	Q_UNUSED(e);

	// caching
	static QImage img = QImage(0,0,QImage::Format_ARGB32);

	// update cache?
	if (img.width() != width() || img.height() != height() || !cached) {

		// now cached
		cached = true;

		// select color
		const QColor c = (selected) ? (QColor(192,0,0)) : (QColor(192,192,0));

		// set-up gradient
		QLinearGradient grad (0, 0, 0, height());
		grad.setColorAt(0, c);
		grad.setColorAt(1, c.darker(150));

		// resize caching image
		img = QImage(width(),height(),QImage::Format_RGB32);
		img.fill(0x000000);
		QPainter p(&img);

		// sizing temporals
		const unsigned int w = width() - 1;
		const unsigned int h = height() - 1;

		// gradient-faded rectangle
		p.setPen(Qt::NoPen);
		p.setBrush(grad);
		p.drawRect(0,0,w,h);

		// draw dark outline
		p.setPen(c.darker(200));
		p.drawLine(0,h,w,h);
		p.drawLine(w,0,w,h);

		// draw bright outline
		p.setPen(c.lighter(200));
		p.drawLine(0,0,w,0);
		p.drawLine(0,0,0,h);

	}

	// render
	QPainter p(this);
	p.drawImage(0,0,img);



}
