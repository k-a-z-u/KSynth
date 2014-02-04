#ifndef SEQUENCERTRACKEVENTS_H
#define SEQUENCERTRACKEVENTS_H

#include <memory>
#include <algorithm>
#include <queue>
#include "midi/MidiEvent.h"
//#include "KLib/memory/FixedPool.h"

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
#include <iostream>
class SequencerTrackEvents : public std::vector< std::unique_ptr<MidiEvent> > {

public:

	/** ctor */
	SequencerTrackEvents() {
		;
	}

	/** dtor */
	~SequencerTrackEvents() {
		;
	}


	/**
	 * add the given midi event to this track.
	 * this method creates a copy of the given midi-event on the heap
	 * which is returned after the function succeeds.
	 */
	MidiEvent* add(const MidiEvent& e) {
		MidiEvent* ep = new MidiEvent(e);
		std::unique_ptr<MidiEvent> ptr(ep);
		push_back( std::move(ptr) );
		return ep;
	}

	/** remove this existring MidiEvent */
	void remove(const MidiEvent& e) {


		auto match = [&e] ( std::unique_ptr<MidiEvent>& other) {
			std::cout << other->asString() << std::endl;
			return e == *other;
		};
		erase(std::remove_if(begin(), end(), match), end());

//		for ( auto it = begin(); it != end(); ) {
//			if (*it == &e) {
//				delete *it;
//				it = erase(it);
//			} else {
//				++it;
//			}
//		}

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
		auto lambda = [] (const std::unique_ptr<MidiEvent>& a, const std::unique_ptr<MidiEvent>& b) {
			return a->delay < b->delay;
		};
		std::sort(begin(), end(), lambda);
	}

	/** fast memory allocation for many many small midi events */
	//K::FixedPool<MidiEvent> memPool;

};

#endif // SEQUENCERTRACKEVENTS_H
