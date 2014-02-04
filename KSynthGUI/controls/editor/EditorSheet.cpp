#include "EditorSheet.h"
#include "Editor.h"
#include "EditorSheetNote.h"

#include "../../misc/Helper.h"

#include <KSynth/Sequencer.h>
#include <QPainter>
#include <QApplication>
#include "EditorSheetHeader.h"

#include <exception>
#include "../../model/Context.h"
#include "../../controller/Controller.h"

EditorSheet::EditorSheet(Editor& editor, SequencerTrack& track, QWidget *parent) :
	QWidget(parent), editor(editor), track(track), drawer(*this), selRect(*this, *this), lastNW4(0) {

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setFocusPolicy(Qt::ClickFocus);

	// convert all midi events (of this track) to notes
	std::string problems = "";
	std::vector<EditorSheetNoteModel> notes = editor.getNotes(track);
	for (EditorSheetNoteModel& en : notes) {
		if (!en.isValid()) {
			problems += "found note-on without corresponding note-off event!\n"; continue;
		}
		new EditorSheetNote(*this, en, this);
	}

	// show message-box with all occured problems
	if (!problems.empty()) {
		editor.getContext().getController()->messageError("error while loading song", "the following errors occured:\n" + problems);
	}

	onZoom();
	drawer.setEnabled(true);
	selRect.setEnabled(true);

}

SequencerTrack& EditorSheet::getTrack() {
	return track;
}

Editor& EditorSheet::getEditor() {
	return editor;
}

QSize EditorSheet::sizeHint() const {
	unsigned int len = editor.getSongLength();
	unsigned int w = editor.getScaler().getObjectWidth(len);
	unsigned int h = (ES_MAX_NOTE - ES_MIN_NOTE) * editor.getScaler().getNH() + 1;
	return QSize(w,h);
}


void EditorSheet::resizeEvent(QResizeEvent*) {
	updateNotes();
}

void EditorSheet::updateNotes() {
	std::cout << "UPDATE NOTES" << std::endl;
	for (QObject* obj : children()) {
		EditorSheetNote* esn = dynamic_cast<EditorSheetNote*>(obj);
		if (esn) {esn->updateSize();}
	}
}

#include <QKeyEvent>
void EditorSheet::keyPressEvent(QKeyEvent* e) {

	unsigned int snapRel = getEditor().getScaler().snap.base;
	unsigned int snapAbs = getEditor().getScaler().snap.enabled ? snapRel : 128;

	// ctrl + A -> select all.
	if (e->key() == Qt::Key_A && e->modifiers() == Qt::ControlModifier) {

		setUnselected();
		for (EditorSheetNote* esn : findChildren<EditorSheetNote*>()) {
			esn->setSelected(true);
			selection.push_back(esn);
		}
		return;

	}

	// delete
	if (e->key() == Qt::Key_Delete) {

		for (EditorSheetNote* esn : selection) {

			// remove both midi events from the SequencerTrack's events
			track.getEvents()->remove( *esn->getModel().getOnEvt() );
			track.getEvents()->remove( *esn->getModel().getOffEvt() );

			// delete the GUI note
			delete esn;
		}

		selection.clear();
		return;

	}

	// move notes up / down
	if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down) {
		for (EditorSheetNote* esn : selection) {
			int dir = (e->key() == Qt::Key_Up) ? (+1) : (-1);
			esn->getModel().setNoteNumber( esn->getModel().getNoteNumber() + dir );
		}
		track.getEvents()->eventsChanged();
	}

	// move notes left/right
	if (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right) {
		for (EditorSheetNote* esn : selection) {
			int dir = (e->key() == Qt::Key_Right) ? (+1) : (-1);
			int steps = dir * getEditor().getScaler().snap.getSteps();
			if (e->modifiers() == Qt::ShiftModifier) {
				esn->getModel().resizeTime(steps, snapRel, snapAbs);
			} else {
				esn->getModel().moveTime(steps, snapRel, snapAbs);
			}
		}
		track.getEvents()->eventsChanged();
	}

}

void EditorSheet::setSelected(EditorSheetNote* note) {
	setUnselected();
	selection.push_back(note);
	note->setSelected(true);
}

void EditorSheet::setUnselected() {
	for (EditorSheetNote* esn : selection) {esn->setSelected(false);}
	selection.clear();
}

void EditorSheet::onSelection(std::vector<QWidget*> list, QMouseEvent* e) {

	// if shift is pressed: add to selection
	if (e->modifiers() != Qt::ShiftModifier) {setUnselected();}

	for (QWidget* w : list) {
		EditorSheetNote* esn = dynamic_cast<EditorSheetNote*>(w);
		if (esn) {
			esn->setSelected(true);
			selection.push_back(esn);
		}
	}
}


#include <QPainter>
void EditorSheet::paintEvent(QPaintEvent* e) {

	Q_UNUSED(e);

	// we use caching for enhanced performance
	static QImage imgTile;

	// check caching
	if (lastNW4 != editor.getScaler().get4thWidth() || imgTile.height() != height()) {
		lastNW4 = editor.getScaler().get4thWidth();

		// NOTE:
		// creating a cached image holding the whole size of the editor sheet
		// would consume a huge amount of memory when the song is very long!
		// thus: we just cache one "tile" (for one bar) and draw several of
		// those cached images side by side

		const unsigned int w = (unsigned int) width() - 1;
		const unsigned int h = (unsigned int) height() - 1;
		//const unsigned int base = editor.getScaler().getBase();
		const unsigned int bw = editor.getScaler().getBarWidth();
		const unsigned int nh = editor.getScaler().getNH();
		const unsigned int sx = editor.getScaler().getSX();
		const unsigned int sy = editor.getScaler().getSY();

		// create new image for ONE base
		// (images are tiled)
		imgTile = QImage(bw, height(), QImage::Format_ARGB32);
		imgTile.fill(0x00000000);
		QPainter p(&imgTile);

		static const QColor lines1(0,0,0,64);
		static const QColor lines2(0,0,0,24);
		static const QColor lines3(0,0,0,80);
		static const QColor black(200,200,200);
		static const QColor white(245,245,245);

		QPen p1; p1.setColor(lines1); p1.setWidth(1);
		QPen p2; p2.setColor(lines2); p2.setWidth(1);
		QPen p3; p3.setColor(lines3); p3.setWidth(2);

		// draw black bars for better orientation
		p.setPen(Qt::NoPen);

		for (int i = 0; i <= ES_MAX_NOTE-ES_MIN_NOTE; ++i) {

			const unsigned int note = (ES_MAX_NOTE - i) % 12;
			if (!( note == 10 || note == 8 || note == 6 || note == 3 || note == 1 )) {
				p.setBrush(white);
			} else {
				p.setBrush(black);
			}

			const int y = i * nh;
			p.drawRect(sx, y, w, nh);

		}

		// draw bars (one full beat)
		p.setPen(QColor(96,96,96));
		const int x = sx;
		p.setPen(p3);
		p.drawLine(x, 0, x, h);			// left border
		p.drawLine(x+bw, 0, x+bw, h);	// right border

		// draw horizontal lines
		p.setPen(p1);
		for (unsigned int y = sy; y < h; y += nh) {
			//if (getEditor().getScaler().getNoteNr(y) % 12 == 11) {p.setPen(p3);} else {p.setPen(p1);}
			p.drawLine(sx,y,w,y);
		}


		// draw 4th sub-lines
		p.setPen(p1);
		if (bw >= 32) {
			for (unsigned int sub = 1; sub < 4; ++sub) {
				int xx = x+bw*sub/4;
				p.drawLine(xx, 0, xx, height());
			}
		}

		// draw 8th sub-lines
		p.setPen(p2);
		if (bw >= 64) {
			for (unsigned int sub = 0; sub < 4; ++sub) {
				int xx = x+bw*sub/4 + bw/8;
				p.drawLine(xx, 0, xx, height());
			}
		}



	}

	// draw all visible tiles
	QPainter p(this);
	const unsigned int bw = editor.getScaler().getBarWidth();
	const unsigned int tileW = imgTile.width();
	const unsigned int tileH = imgTile.height();

	for (unsigned int x = 0; x < (unsigned int) width(); x += bw) {

		// is this tile visible?
		QRect tileRect(x,0,tileW,tileH);
		if (!e->region().contains(tileRect)) {continue;}

		// draw
		p.drawImage(x,0,imgTile);

	}

	// draw active selection rectangle (if any)
	selRect.draw(p);

}

void EditorSheet::onZoom() {

	// this will invalidate the layout
	// and the layout calls sizeHint() to get the widget's new size
	this->updateGeometry();

	updateNotes();


}
