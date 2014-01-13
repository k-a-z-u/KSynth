/*
 * MidiFile.h
 *
 *  Created on: Nov 2, 2013
 *      Author: kazu
 */

#ifndef MIDIFILE_H_
#define MIDIFILE_H_

#include <klib/fs/File.h>
#include "MidiTrack.h"

/**
 * represents one midi-file with its
 * settings, multiple tracks and events
 */
class MidiFile {

public:

	/** ctor */
	MidiFile() : speed(0) {;}

	/** get track by number */
	MidiTrack& getTrack(unsigned int idx) {
		return tracks[idx];
	}

	/** get vector of all tracks */
	const std::vector<MidiTrack>& getTracks() const {
		return tracks;
	}

	int getSpeed() {
		return speed;
	}

private:

	friend class MidiParser;

	/** add a new track to this file */
	void add(MidiTrack t) {
		tracks.push_back(t);
	}

	/** all tracks within this file */
	std::vector<MidiTrack> tracks;

	/** ??? */
	int speed;

};


#endif /* MIDIFILE_H_ */
