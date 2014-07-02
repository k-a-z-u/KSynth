#ifndef MIDICONSTANTS_H
#define MIDICONSTANTS_H

/**
  * this file contains some enums for constant
  * values of a midi file like version-types,
  * event-types, instruments, etc.
  */

/** all known midi versions */
enum class MidiVersion {
	SINGLE_TRACK,			// midi V0
	MULTI_TRACK_SYNC,		// midi V1
	MULTI_TRACK_ASYNC		// midi V2
};

/**
 * all possible midi-events as enum
 */
enum class MidiEventType {
	NOTE_OFF,
	NOTE_ON,
	POLYPHONIC_KEY_PRESSURE,
	CONTROL_CHANGE,
	PROGRAM_CHANGE,
	CHANNEL_PRESSURE,
	PITCH_WHEEL_CHANGE,
	META,
};


#endif // MIDICONSTANTS_H
