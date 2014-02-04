#include "EditorSheetNote.h"

#include "EditorSheet.h"
#include "Editor.h"
#include "KSynth/SequencerTrack.h"

EditorSheetNote::EditorSheetNote(EditorSheet& sheet, const EditorSheetNoteModel& model, QWidget *parent) :
	Grabable(parent), sheet(sheet), model(model), selected(false), cached(false) {

	setFocusPolicy(Qt::ClickFocus);

	updateSize();
	setMouseTracking(true);

	// register for model changed events
	connect(&(this->model), SIGNAL(changed()), this, SLOT(updateSize()));

}

#include <iostream>
void EditorSheetNote::updateSize() {

	// set geometry from underlying model
	setGeometry( sheet.getEditor().getScaler().getNote(model) );

	setGrabY(false);
	//setSnapX( sheet.getEditor().getScaler().getBarWidth() / 4 );
	setSnapY(8);


}

#include <iostream>
#include <QFocusEvent>
void EditorSheetNote::focusInEvent(QFocusEvent* e) {
	Q_UNUSED(e);
	sheet.setSelected(this);
}

void EditorSheetNote::focusOutEvent(QFocusEvent* e) {
	Q_UNUSED(e);
}

void EditorSheetNote::onGrab(int x, int y, int w, int h) {

	Q_UNUSED(h);
	sheet.getEditor().getScaler().setNote(model, x,y,w);
	setGeometry( sheet.getEditor().getScaler().getNote(model) );

}

void EditorSheetNote::onGrabDone(int x, int y, int w, int h) {

	Q_UNUSED(h);

	// change note's model: number and on/off timing
	sheet.getEditor().getScaler().setNote(model, x,y,w);

	// update the geometry of the note
	setGeometry( sheet.getEditor().getScaler().getNote(model) );

	// as the timings (might) have changed,
	//the track's MidiEvents need to be sorted again
	sheet.getTrack().getEvents()->eventsChanged();

	// refresh GUI
	updateSize();

}

void EditorSheetNote::setSelected(bool selected) {
	bool changed = this->selected != selected;
	this->selected = selected;
	if (changed) {cached = false; emit repaint();}
}

EditorSheetNoteModel& EditorSheetNote::getModel() {
	return model;
}

#include <QPainter>
#include "EditorHelper.h"
void EditorSheetNote::paintEvent(QPaintEvent* e) {

	//std::cout << "REPAINT NOTE" << std::endl;
	Q_UNUSED(e);

	QPainter p(this);

	// sizing temporals
	const unsigned int sx = 0;
	//const unsigned int sy = 1;
	const unsigned int w = width() - 1;
	const unsigned int h = height() - 1;

//	// caching
//	static QImage img;// = QImage(0,0,QImage::Format_ARGB32);

//	// update cache?
//	if (img.width() != width() || img.height() != height() || !cached) {
//		cached = true;

		// select color
		const QColor c = (selected) ? (QColor(220,32,0)) : (QColor(220,220,0));

		// set-up gradient
		QLinearGradient grad (0, 0, 0, h);
		grad.setColorAt(0, c);
		grad.setColorAt(1, c.darker(200));

//		// resize caching image
//		img = QImage(width(),height(),QImage::Format_ARGB32);
//		img.fill(0x00000000);
//		QPainter p(&img);

		// gradient-faded rectangle
		p.setPen(Qt::NoPen);
		p.setBrush(grad);
		p.drawRect(sx+1,sx+1,w-1,h-1);

		EditorHelper::drawBorder3D(p,1,1,w,h);

//	}

//	// render
//	p.drawImage(0,0,img);



}
