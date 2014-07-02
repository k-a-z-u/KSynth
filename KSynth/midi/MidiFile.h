/*
 * MidiFile.h
 *
 *  Created on: Nov 2, 2013
 *      Author: kazu
 */

#ifndef MIDIFILE_H_
#define MIDIFILE_H_

#include <KLib/fs/File.h>
#include "MidiTrack.h"
#include "MidiConstants.h"

/**
 * represents one midi-file with its
 * settings, multiple tracks and events
 */
class MidiFile {

public:

	/** ctor */
	MidiFile() : tickDiv(0) {;}

	/** get track by number */
	MidiTrack& getTrack(unsigned int idx) {
		return tracks[idx];
	}

	/** get vector of all tracks */
	const std::vector<MidiTrack>& getTracks() const {
		return tracks;
	}

	/** get the number of ticks per delta-time (?) */
	unsigned int getTickDiv() const {
		return tickDiv;
	}

	/** the midi's version number (normally 0 or 1) */
	MidiVersion getVersion() const {
		return version;
	}

	/** remove all tracks from the midi file */
	void deleteTracks() {
		tracks.clear();
	}

private:

	friend class MidiParser;

	/** add a new track to this file */
	void add(MidiTrack t) {
		tracks.push_back(t);
	}

	/** all tracks within this file */
	std::vector<MidiTrack> tracks;

	/** the number of ticks to use per delta-time (?) */
	unsigned int tickDiv;

	/** the midi version */
	MidiVersion version;

};


#endif /* MIDIFILE_H_ */
