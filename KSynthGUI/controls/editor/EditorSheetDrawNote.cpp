#include "EditorSheetDrawNote.h"

#include <QMouseEvent>

#include "EditorSheet.h"
#include "Editor.h"
#include "EditorSheetNote.h"

#include <KSynth/SequencerTrack.h>

EditorSheetDrawNote::EditorSheetDrawNote(EditorSheet& sheet) :
	sheet(sheet) {
	;
}

void EditorSheetDrawNote::setEnabled(bool enabled) {
	if (enabled) {
		sheet.installEventFilter(this);
	} else {
		sheet.removeEventFilter(this);
	}
}

bool EditorSheetDrawNote::eventFilter(QObject* o, QEvent* e) {
	if (o != &sheet) {return false;}
	if (e->type() == QEvent::MouseMove)				{onMouseMove( (QMouseEvent*) e );}
	if (e->type() == QEvent::MouseButtonPress)		{onMousePress( (QMouseEvent*) e );}
	if (e->type() == QEvent::MouseButtonRelease)	{onMouseRelease( (QMouseEvent*) e );}
	return false;
}

void EditorSheetDrawNote::onMouseMove(QMouseEvent* e) {

	// only act if mouse is currently down
	if (!mouse.isDown) {return;}

	// resize the note (change note-off event)
	int time = sheet.getEditor().getScaler().getNoteDelay(e->x());
	mouse.noteOff->setDelay(time);

	// update the note's visible part
	mouse.sheetNote->updateSize();

}

void EditorSheetDrawNote::onMousePress(QMouseEvent* e) {

	// only allow note drawing if editor is in drawing mode!
	// FIXME: better solution instead of checking here? like: enable/disable (setEnabled())
	// the filter on mode changes??
	if (sheet.getEditor().getMode() != EditorMode::DRAW) {return;}

	mouse.isDown = true;
	mouse.start = e->pos();

	// create two new midi events for note-on and note-off
	int noteNr = sheet.getEditor().getScaler().getNoteNr(e->y());
	int time = sheet.getEditor().getScaler().getNoteDelay(e->x());
	MidiEvent noteOn(time,0,0,0);  noteOn.setType(MidiEventType::NOTE_ON);   noteOn.setData1(noteNr);  noteOn.setData2(100);
	MidiEvent noteOff(time,0,0,0); noteOff.setType(MidiEventType::NOTE_OFF); noteOff.setData1(noteNr); noteOff.setData2(0);
	mouse.noteOn = sheet.getTrack().getEvents()->add(noteOn);
	mouse.noteOff = sheet.getTrack().getEvents()->add(noteOff);

	// create a new GUI note and attach it to the EditorSheet
	mouse.sheetNote = new EditorSheetNote(sheet, EditorNote(mouse.noteOn, mouse.noteOff), &sheet);
	mouse.sheetNote->setVisible(true);

}

void EditorSheetDrawNote::onMouseRelease(QMouseEvent* e) {

	Q_UNUSED(e);
	mouse.isDown = false;

	// we finished drawing a new note.
	// the new note (note-on/note-off event) belongs to the SequenerTrack.
	// thus: we do not need those two pointers any more
	mouse.noteOn = nullptr;
	mouse.noteOff = nullptr;

	// re-sort all entries within the sequencer track!
	sheet.getTrack().getEvents()->eventsChanged();

	//sheet.updateNotes();

}
