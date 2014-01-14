#ifndef EDITORNOTE_H
#define EDITORNOTE_H

#include <KSynth/midi/MidiEvent.h>

/**
 * combines a MIDI note-on with the corresponding note-off event
 * to get the time-range for the note -> rendering within gui, editing, ...
 */
struct EditorNote {

	/** the event starting this note */
	MidiEvent* on;

	/** the event stopping this note */
	MidiEvent* off;

	/** ctor */
	EditorNote() : on(nullptr), off(nullptr) {;}

	/** ctor */
	EditorNote(MidiEvent* on) : on(on), off(nullptr) {;}

	/** get the length (in multiples of 128th notes) */
	unsigned int getLength() {
		return off->getDelay() - on->getDelay();
	}

};

#endif // EDITORNOTE_H
