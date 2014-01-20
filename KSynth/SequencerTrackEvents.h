#ifndef SEQUENCERTRACKEVENTS_H
#define SEQUENCERTRACKEVENTS_H

#include <algorithm>
#include <queue>
#include "midi/MidiEvent.h"

/**
 * this class provides a storage solution for all midi events within
 * on track. as the events must appear in order (depending on their delay),
 * we need to ensure the events are sorted by their occurence-time at all
 * times.
 *
 * when adding, editing or removing events, the implementation ensures
 * all events are properly sorted.
 *
 */
class SequencerTrackEvents : public std::vector<MidiEvent*> {

public:

	/** ctor */
	SequencerTrackEvents() {;}

	/** dtor */
	~SequencerTrackEvents() {;}


	/**
	 * add the given midi event to this track.
	 * this method creates a copy of the given midi-event on the heap
	 * which is returned after the function succeeds.
	 */
	MidiEvent* add(const MidiEvent& e) {
		MidiEvent* ep = new MidiEvent(e);
		push_back( ep );
		sort();
		return ep;
	}

	/** remove this existring MidiEvent */
	void remove(const MidiEvent& e) {
		auto match = [&e] (const MidiEvent* other) {return e == *other;};
		erase(std::remove_if(begin(), end(), match), end());
	}

	/**
	 * call this method when events are changed using external code
	 * and the queue thus needs to be sorted again
	 */
	void eventsChanged() {
		sort();
	}



private:

	/**
	 * this method ensures the queue is properly sorted after any
	 * change like edit, remove, insert...
	 * TODO: use queue data-structure to speed things up and remove
	 * this costly "complete re-sort"
	 */
	void sort() {
		auto lambda = [] (const MidiEvent* a, const MidiEvent* b) {return a->delay < b->delay;};
		std::sort(begin(), end(), lambda);
	}

};

#endif // SEQUENCERTRACKEVENTS_H
