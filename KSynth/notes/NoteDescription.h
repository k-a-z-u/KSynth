/*
 * NoteDescription.h
 *
 *  Created on: Oct 24, 2013
 *      Author: kazu
 */

#ifndef NOTEDESCRIPTION_H_
#define NOTEDESCRIPTION_H_

#include "Note.h"
#include "../misc/DataTypes.h"

/**
 * describes how to play a note
 * start-time, volume, ...
 */
struct NoteDescription {

	/** the note to play */
	Note note;

	/** the volume to use for playing */
	Volume vol;

	/** the note's age */
	SampleFrame age;

	/** ctor */
	NoteDescription() : note(0), vol(0), age(0) {;}

	/** ctor */
	NoteDescription(Note note, Volume vol) : note(note), vol(vol), age(0) {;}

};

#include <unordered_map>
namespace std {
  template<> struct hash<Note> {
    size_t operator()(const Note & note) const {
    	return hash<int>()(note.getNoteNr());
    }
  };
}

#endif /* NOTEDESCRIPTION_H_ */
