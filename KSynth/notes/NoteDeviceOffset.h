/*
 * NoteDeviceOffset.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef NOTEDEVICEOFFSET_H_
#define NOTEDEVICEOFFSET_H_

#include "NoteDevice.h"

/**
 * add an offset to all nodes played by this devices
 */
class NoteDeviceOffset : public NoteDevice {

public:

	/** ctor */
	NoteDeviceOffset(NoteDevice& src) : src(src), noteOffset(0) {;}

	/** change the note offset */
	void setOffset(int noteOffset) {
		this->noteOffset = noteOffset;
	}

	void startNote(Note note, Time start, Volume volume) override {
		src.startNote(note.shift(noteOffset), start, volume);
	}

	void stopNote(Note note) override {
		src.stopNote(note.shift(noteOffset));
	}



private:

	/** the original device */
	NoteDevice& src;

	int noteOffset;

};



#endif /* NOTEDEVICEOFFSET_H_ */
