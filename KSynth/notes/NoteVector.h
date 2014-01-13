/*
 * NoteVector.h
 *
 *  Created on: Oct 24, 2013
 *      Author: kazu
 */

#ifndef NOTEVECTOR_H_
#define NOTEVECTOR_H_

#include "NoteDescription.h"
#include <list>

/**
 * store subclasses of Note within this vector
 * that support fast adding, changing and removing
 */
template <typename T> class NoteVector {

public:

	/** ctor */
	NoteVector() {
		;
	}

	/** remove all entries */
	void clear() {
		entries.clear();
	}

	/** add a new entry */
	void add(const T& next) {

		// replace existing entry?
		for (T& old : entries) {
			if (old.note == next.note) {old = next; return;}
		}

		// add new entry
		entries.push_back(next);

	}

	T* get(Note n) {
		for (T& nd : entries) {
			if (nd.note == n) {return &nd;}
		}
		return nullptr;
	}

	/** remove the given note from the vector */
	void remove(Note note) {
		auto it = entries.begin();
		for ( ; it != entries.end(); ) {
			if (it->note == note) {
				it = entries.erase(it);
			} else {
				++it;
			}
		}
	}

	/** remove all notes that have been stopped at least 1 second ago */
	void cleanup(SampleRate age) {
		for (auto it = entries.begin(); it != entries.end(); ) {
			if (it->age >= age) {
				it = entries.erase(it);
			} else {
				++it;
			}
		}
	}

	/** get all entries within the vector */
	std::list<T>& getEntries()  {
		return entries;
	}

private:

	/** the vector of entries */
	std::list<T> entries;

};


#endif /* NOTEVECTOR_H_ */
