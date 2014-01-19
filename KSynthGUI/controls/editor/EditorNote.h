#ifndef EDITORNOTE_H
#define EDITORNOTE_H

#include <KSynth/midi/MidiEvent.h>

/**
 * combines a MIDI note-on with the corresponding note-off event
 * to get the time-range for the note -> rendering within gui, editing, ...
 *
 * both events directly point into the SequencerTrack's event-queue and
 * may thus be modified.
 *
 * if timing values of those events are changed, the SequencerTrack's
 * events need to be sorted again for correct playback!
 */
struct EditorNote {

	/** the event starting this note */
	MidiEvent* on;

	/** the event stopping this note */
	MidiEvent* off;

	/** empty ctor */
	EditorNote() : on(nullptr), off(nullptr) {;}

	/** ctor. set only note-on event */
	EditorNote(MidiEvent* on) : on(on), off(nullptr) {;}

	/** get the length (in multiples of 128th notes) */
	unsigned int getLength() {
		return off->getDelay() - on->getDelay();
	}

};

#endif // EDITORNOTE_H
