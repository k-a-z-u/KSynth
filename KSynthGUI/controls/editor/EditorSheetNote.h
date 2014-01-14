#ifndef EDITORSHEETNOTE_H
#define EDITORSHEETNOTE_H

#include <QWidget>
#include "EditorNote.h"
class EditorSheet;

/**
 * represents an editable note element within the sheet.
 * a note can be moved, deleted, created, resized, etc...
 *
 * this element uses two underlying MIDI events for note-on
 * and note-off.
 *
 */
class EditorSheetNote : public QWidget {
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

	friend class EditorSheet;
	/** mark the note as selected */
	void setSelected(bool selected);


private:

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
