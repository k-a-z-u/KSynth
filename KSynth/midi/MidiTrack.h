/*
 * MidiTrack.h
 *
 *  Created on: Nov 2, 2013
 *      Author: kazu
 */

#ifndef MIDITRACK_H_
#define MIDITRACK_H_

#include <vector>
#include "MidiEvent.h"
#include "MidiInstruments.h"


/**
 * represents one midi-track
 * and all its events
 */
class MidiTrack {

public:

	/** ctor */
	MidiTrack() : head(0) {;}

	/** get the next event but keep the position */
	MidiEvent peekEvent() {
		if (events.size() <= head) {return MidiEvent();}
		return events[head];
	}

	/** get the next event */
	MidiEvent popEvent() {
		if (events.size() <= head) {return MidiEvent();}
		return events[head++];
	}

	/** get all events from this track */
	const std::vector<MidiEvent>& getEvents() const {
		return events;
	}

	/** get the number of events */
	unsigned int getNumEvents() {
		return (unsigned int) events.size();
	}

	/** get the track's name (if any) */
	const std::string& getName() const {
		return name;
	}

	/** does this track contain any events? */
	bool isEmpty() const {
		return events.empty();
	}

	/** this will return a string representation for each instrument used within this track */
	std::vector<std::string> getInstrumentsAsString() const {
		std::vector<std::string> instruments;
		for (const MidiEvent& evt : events) {
			if (evt.getType() == MidiEventType::PROGRAM_CHANGE) {
				instruments.push_back( midiInstruments[evt.d1] );
			}
		}
		return instruments;
	}

private:

	friend class MidiParser;

	/** append one event */
	void add(MidiEvent e) {
		events.push_back(e);
	}

	/** all events within this track */
	std::vector<MidiEvent> events;

	/** the next event to pop */
	unsigned int head;

	/** the track's name (if any) */
	std::string name;

};


#endif /* MIDITRACK_H_ */
