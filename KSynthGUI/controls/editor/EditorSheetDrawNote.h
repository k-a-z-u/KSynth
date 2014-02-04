#ifndef EDITORSHEETDRAWNOTE_H
#define EDITORSHEETDRAWNOTE_H

#include <QWidget>
#include "EditorSheetNoteModel.h"
class EditorSheetNote;
class EditorSheet;
class QMouseEvent;
#include <QPoint>

/**
 * this class listens for mouse events within the EditorSheet
 * and provides note-drawing (adding new notes)
 */
class EditorSheetDrawNote : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	EditorSheetDrawNote(EditorSheet& sheet);

	/** enable/disable drawing facilities */
	void setEnabled(bool enabled);


protected:

	bool eventFilter(QObject*, QEvent*) override;


private:

	void onMouseMove(QMouseEvent*);
	void onMousePress(QMouseEvent*);
	void onMouseRelease(QMouseEvent*);

	/** snap the currently drawn note (if snapping is enabled) */
	void mouseSnapNote();

private:

	/** the sheet this drawer belongs to */
	EditorSheet& sheet;

	struct Mouse {

		/** the starting position (mouse went down here) */
		QPoint start;

		/** is the mouse currently down? */
		bool isDown;

		/** the note-on event for the currently drawing note */
		MidiEvent* noteOn;

		/** the note-off event for the currently drawing note */
		MidiEvent* noteOff;

		/** the GUI representation of the currently drawing note (uses both events above) */
		EditorSheetNote* sheetNote;

		Mouse() : isDown(false), noteOn(nullptr), noteOff(nullptr), sheetNote(nullptr) {;}

	} mouse;

};

#endif // EDITORSHEETDRAWNOTE_H
