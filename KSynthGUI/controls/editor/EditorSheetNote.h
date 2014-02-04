#ifndef EDITORSHEETNOTE_H
#define EDITORSHEETNOTE_H

#include <QWidget>
#include "EditorSheetNoteModel.h"
class EditorSheet;
#include "Grabable.h"

/**
 * represents an editable note element within the sheet.
 * a note can be moved, deleted, created, resized, etc...
 *
 * this element uses two underlying MIDI events:
 * one for note-on, and one for note-off.
 *
 */
class EditorSheetNote : public Grabable {
	Q_OBJECT

public:

	/**
	 * ctor
	 * @param sheet the EditorSheet this note belongs to (shall be displayed within)
	 * @param note describes the note using two midi events. !! this struct will be copied !!
	 */
	explicit EditorSheetNote(EditorSheet& sheet, const EditorSheetNoteModel& model, QWidget *parent = 0);

protected:

	void paintEvent(QPaintEvent*) override;

	void focusInEvent(QFocusEvent*) override;
	void focusOutEvent(QFocusEvent*) override;

	void onGrab(int x, int y, int w, int h) override;
	void onGrabDone(int x, int y, int w, int h) override;

	friend class EditorSheet;

	/** mark the note as selected */
	void setSelected(bool selected);

	/** get the displayed MidiEvents (note on/note off) */
	EditorSheetNoteModel& getModel();


private:

	/** the EditorSheet this note belongs to */
	EditorSheet& sheet;

	/** the note (described using two midi events */
	EditorSheetNoteModel model;

	/** is the note currently selected? */
	bool selected;

	/** image currently cached? */
	bool cached;


public slots:

	/** update the note's size and position */
	void updateSize();

};

#endif // EDITORSHEETNOTE_H
