/*
 * SequencerListener.h
 *
 *  Created on: Dec 28, 2013
 *      Author: kazu
 */

#ifndef SEQUENCERLISTENER_H_
#define SEQUENCERLISTENER_H_

#include "misc/DataTypes.h"

/**
 * available status variants for the sequencer
 */
enum class SequencerStatus {
	PLAYING,
	STOPPED,
	RECORDING,
};

/** listen for sequencer events */
class SequencerBeatListener {

public:

	/** dtor */
	virtual ~SequencerBeatListener() {;}

	/** called for every 128th beat */
	virtual void onBeat(Beat beat, Time time) = 0;

};

/** listen for status changes */
class SequencerStatusListener {

public:

	/** dtor */
	virtual ~SequencerStatusListener() {;}

	/** the sequencer changed its status */
	virtual void onStatusChange(SequencerStatus status) = 0;

	/** the sequencer settings (like bpm) have changed */
	virtual void onSettingsChange() = 0;

};

/** listen for track changes */
class SequencerTrackListener {

public:

	/** dtor */
	virtual ~SequencerTrackListener() {;}

	/** the list of tracks within the sequencer has changed */
	virtual void onTracksChanged() = 0;

};



#endif /* SEQUENCERLISTENER_H_ */
