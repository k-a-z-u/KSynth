/*
 * RtMidiEventListener.h
 *
 *  Created on: Dec 27, 2013
 *      Author: kazu
 */

#ifndef RTMIDIEVENTLISTENER_H_
#define RTMIDIEVENTLISTENER_H_

class RtMidiInDevice;

#include "../MidiEvent.h"

/**
 * listen for incoming midi events
 */
class RtMidiEventListener {

public:

	/** dtor */
	virtual ~RtMidiEventListener() {;}

	/**
	 * called for every received MidiEvent
	 * @param src the source device that created the given event
	 * @param evt the midi event
	 */
	virtual void onMidiEvent(RtMidiInDevice& src, MidiEvent evt) = 0;

};


#endif /* MIDIEVENTLISTENER_H_ */
