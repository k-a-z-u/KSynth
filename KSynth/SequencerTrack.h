#ifndef SEQUENCERTRACK_H
#define SEQUENCERTRACK_H

#include "SequencerTrackEvents.h"
#include "notes/NoteDevice.h"
#include "misc/DataTypes.h"

/**
 * a Sequencer track represents one track (some kind of logical unit) containing
 * midi events, ordered by their delay.
 *
 * Synthesizers can be bound to those tracks in order to receive all their events
 * and generate appropriate audio output.
 *
 * SequencerTracks can be modified:
 *	add / delete / modify midi events like notes (note on / note off)
 *
 */
class SequencerTrack {


public:

	/** ctor */
	SequencerTrack() : curEventIdx(0), dev(nullptr), name("") {;}


	/** get the track's name */
	const std::string& getName() const {
		return name;
	}

	/** set this track's name */
	void setName(const std::string& name) {
		this->name = name;
	}

	/** get a textual description for this track */
	const std::string& getDescription() const {
		return desc;
	}

	/** set a textual description for this track */
	void setDescription(const std::string& desc) {
		this->desc = desc;
	}

	/** get the attached note device, or nullptr if none */
	NoteDevice* getDevice() const {
		return dev;
	}

	/** get this track's length in multiples of 128th notes */
	TimeBase128 getLength() const {
		if (events.empty()) {return 0;}
		return events[events.size() - 1]->delay;
	}

	/** attach a new note-device */
	void setDevice(NoteDevice* dev) {

		// stop all notes within the old device
		if (this->dev) {this->dev->stopNotes();}

		// attach new device
		this->dev = dev;

	}

	/** get the vector containing all midi-events */
	SequencerTrackEvents* getEvents() {
		return &events;
	}


private:

	friend class Sequencer;

	/** track the current event's index */
	unsigned int curEventIdx;

	/** all events within this track */
	SequencerTrackEvents events;

	/** the device attached to this track (if any) */
	NoteDevice* dev;

	/** the track's name */
	std::string name;

	/** some textual description for this track */
	std::string desc;



};


#endif // SEQUENCERTRACK_H
