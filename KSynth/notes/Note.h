/*
 * Note.h
 *
 *  Created on: Oct 24, 2013
 *      Author: kazu
 */

#ifndef NOTE_H_
#define NOTE_H_

#include <cmath>
#include "../misc/DataTypes.h"

#define noteToFreq(note)	Frequency( pow(2, (( (int)noteNr - 69) / 12.0)) * 440.0 )
#define freqToNote(freq)	12*log2(freq/440)


// TODO
// FIXME
// Look-up-table for notenr <-> frequency

class Note {

public:

	/** ctor */
	Note() : noteNr(0), freq(0) {
		;
	}

	/** ctor */
	Note(unsigned int noteNr) : noteNr(noteNr), freq(noteToFreq(noteNr)) {
		;
	}

	/** convert note-nr to frequency */
	Frequency getFrequency() const {
		return freq;
	}

	/** shift this note by the given amount of notes [12 = octave] */
	Note shift(int notes) {
		return Note(noteNr + notes);
	}

	/** change the current note nr and frequency */
	void setNoteNr(unsigned int noteNr) {
		this->noteNr = noteNr;
		this->freq = noteToFreq(noteNr);
	}

	/** get the current note number */
	inline unsigned int getNoteNr() const {return this->noteNr;}

	/** assign from note number */
	void operator = (const unsigned int noteNr) {
		setNoteNr(noteNr);
	}

	/** compare with other note */
	bool operator == (const Note other) const {
		return other.noteNr == noteNr;
	}

private:

	/** the note number (see midi format) */
	unsigned int noteNr;

	/** the corresponding frequency */
	Frequency freq;

};





#endif /* NOTE_H_ */
