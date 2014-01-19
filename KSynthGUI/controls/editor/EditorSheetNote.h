#ifndef EDITORSHEETNOTE_H
#define EDITORSHEETNOTE_H

#include <QWidget>
#include "EditorNote.h"
class EditorSheet;
#include "Grabable.h"

/**
 * represents an editable note element within the sheet.
 * a note can be moved, deleted, created, resized, etc...
 *
 * this element uses two underlying MIDI events for note-on
 * and note-off.
 *
 */
class EditorSheetNote : public Grabable {
	Q_OBJECT

public:

	/** ctor */
	explicit EditorSheetNote(EditorSheet& sheet, EditorNote note, QWidget *parent = 0);

	/** update the note's size and position */
	void updateSize();

protected:

	void paintEvent(QPaintEvent*) override;

	void focusInEvent(QFocusEvent*) override;
	void focusOutEvent(QFocusEvent*) override;

	//void mouseMoveEvent(QMouseEvent*) override;
	//void mousePressEvent(QMouseEvent*) override;
	//void mouseReleaseEvent(QMouseEvent*) override;

	void onGrab(int x, int y, int w, int h) override;
	void onGrabDone(int x, int y, int w, int h) override;

	friend class EditorSheet;
	/** mark the note as selected */
	void setSelected(bool selected);

	/** get the displayed MidiEvents (note on/note off) */
	const EditorNote& getNote() const;

private:

//	/** returns: 1: mouse within left-grab, 2: mouse within right grab, 0: not within grab */
//	int mouseInGrab(const QPoint& pos);



	/** the EditorSheet this note belongs to */
	EditorSheet& sheet;

	/** the note (described using two midi events */
	EditorNote note;

	/** is the note currently selected? */
	bool selected;

	/** image currently cached? */
	bool cached;


signals:

public slots:

};

#endif // EDITORSHEETNOTE_H
