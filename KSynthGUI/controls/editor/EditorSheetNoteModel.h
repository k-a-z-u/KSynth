#ifndef EDITORSHEETNOTEMODEL_H
#define EDITORSHEETNOTEMODEL_H

#include <KSynth/midi/MidiEvent.h>
#include <QObject>

/**
 * combines a MIDI note-on with the corresponding note-off event
 * to get the time-range for the note -> rendering within gui, editing, ...
 *
 * this struct just ackts like a glue between both events. as we use pointers
 * to access those, and the pointer belong to the SequencerTrack (backend)
 * the struct may be copied, etc...
 *
 * both events directly point into the SequencerTrack's event-queue and
 * may thus be modified.
 *
 * if timing values of those events are changed, the SequencerTrack's
 * events need to be sorted again for correct playback!
 *
 */
class EditorSheetNoteModel : public QObject {
	Q_OBJECT

public:

	/** empty ctor */
	EditorSheetNoteModel();

	/** ctor. set only note-on event */
	EditorSheetNoteModel(MidiEvent* on);

	/** ctor. set both events */
	EditorSheetNoteModel(MidiEvent* on, MidiEvent* off);

	/** copy ctor */
	EditorSheetNoteModel(const EditorSheetNoteModel& other);



	/** get the length (in multiples of 128th notes) */
	unsigned int getLength() const;

	/** just for sanity checking.. every note-on needs corresponding note-off! */
	bool isValid() const;

	/** reset this note. set both events to nullptr */
	void reset();


	/** get the note-number for this note */
	unsigned int getNoteNumber() const;


	/** get the MidiEvent for note-on */
	const MidiEvent* getOnEvt() const;

	/** get the MidiEvent for note-off */
	const MidiEvent* getOffEvt() const;

	/** set the MidiEvent for note-off */
	void setOffEvt(MidiEvent* evt);


	/** set the note number for both events (on and off) */
	void setNoteNumber(unsigned int noteNumber);


	/**
	 * set the time values for note-on and note-off
	 * @param absGran snap on and off to multiple of the given granularity [1-128]
	 */
	void setTimes(TimeBase128 on, TimeBase128 off, unsigned int absGran = 128);

	/**
	 * move note (on/off) "left" or "right"
	 * @param absGran snap result (time + offset) to multiple of the given granularity [1-128]
	 * @param relGran snap the given offeset to a multiple of the given granularity [1-128]
	 */
	void moveTime(TimeOffset128 offset, unsigned int relGran = 128, unsigned int absGran = 128);

	/** resize the note (longer, shorter)
	 * @param absGran snap result (time + offset) to multiple of the given granularity [1-128]
	 * @param relGran snap the given offeset to a multiple of the given granularity [1-128]
	 */
	void resizeTime(TimeOffset128 offset, unsigned int relGran = 128, unsigned int absGran = 128);


signals:

	/** the model has changed */
	void changed();


private:

	/** the event starting this note */
	MidiEvent* on;

	/** the event stopping this note */
	MidiEvent* off;


};



#endif // EDITORSHEETNOTEMODEL_H
