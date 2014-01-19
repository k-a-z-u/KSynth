#include "EditorSheet.h"
#include "Editor.h"
#include "EditorSheetNote.h"

#include "../../misc/Helper.h"

#include <KSynth/Sequencer.h>
#include <QPainter>
#include <QApplication>
#include "EditorSheetHeader.h"



EditorSheet::EditorSheet(Editor& editor, SequencerTrack& track, QWidget *parent) :
	QWidget(parent), editor(editor), track(track) {

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setFocusPolicy(Qt::ClickFocus);

	// convert all midi events (of this track) to notes
	std::vector<EditorNote> notes = editor.getNotes(track);
	for (EditorNote& en : notes) {
		new EditorSheetNote(*this, en, this);
	}

	onZoom();

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

void EditorSheet::focusInEvent(QFocusEvent*) {
	setUnselected();
}

void EditorSheet::focusOutEvent(QFocusEvent*) {
	;
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
	if (e->key() == Qt::Key_Delete) {
		for (EditorSheetNote* esn : selection) {
			delete esn;
			track.getEvents()->remove(*esn->getNote().on);
			track.getEvents()->remove(*esn->getNote().off);
		}
	}
	selection.clear();
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



#include <QPainter>
void EditorSheet::paintEvent(QPaintEvent* e) {

	Q_UNUSED(e);

	// we use caching for enhanced performance
	static QImage img = QImage(0, 0, QImage::Format_ARGB32);

	// check caching
	if (img.width() != width() || img.height() != height()) {

		// create new image
		img = QImage(width(), height(), QImage::Format_ARGB32);
		img.fill(0x00000000);
		QPainter p(&img);

		static QColor lines(164,164,164);
		static QColor black(200,200,200);
		static QColor white(245,245,245);

		QPen p1; p1.setColor(lines); p1.setWidth(1);
		QPen p2; p2.setColor(lines); p2.setWidth(2);

		unsigned int w = (unsigned int) width();
		unsigned int h = (unsigned int) height();
		unsigned int base = editor.getScaler().getBase();
		unsigned int bw = editor.getScaler().getBarWidth();
		unsigned int nw = editor.getScaler().getNW();
		unsigned int nh = editor.getScaler().getNH();
		unsigned int sx = editor.getScaler().getSX();
		unsigned int sy = editor.getScaler().getSY();

		unsigned int len = editor.getSongLength();
		unsigned int bars = editor.getScaler().getNumBars(len);

		// draw black bars for better orientation
		p.setPen(Qt::NoPen);

		for (int i = 0; i <= ES_MAX_NOTE-ES_MIN_NOTE; ++i) {
			unsigned int note = ES_MAX_NOTE - i; note %= 12;
			if (!( note == 10 || note == 8 || note == 6 || note == 3 || note == 1 )) {
				p.setBrush(white);
			} else {
				p.setBrush(black);
			}
			int y = i * nh;
			p.drawRect(sx, y, w, nh);
		}

		// draw bars (one full beat)
		p.setPen(QColor(96,96,96));
		for (unsigned int bar = 0; bar < bars; ++bar) {

			int x = sx + bw*bar;
			p.setPen(p2);
			p.drawLine(x, 0, x, height());

			// draw sub lines
			p.setPen(p1);
			for (unsigned int sub = 1; sub < base; ++sub) {
				int xx = x+bw*sub/base;
				p.drawLine(xx, 0, xx, height());
			}

		}

		// draw horizontal lines
		p.setPen(p1);
		for (unsigned int y = sy; y < h; y += nh) {
			p.drawLine(sx,y,w,y);
		}

	}

	// draw cached image
	QPainter p(this);
	p.drawImage(0,0,img);

}

void EditorSheet::onZoom() {

	// this will invalidate the layout
	// and the layout calls sizeHint() to get the widget's new size
	this->updateGeometry();

	updateNotes();


}
