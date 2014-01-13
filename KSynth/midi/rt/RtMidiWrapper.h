/*
 * RtMidiWrapper.h
 *
 *  Created on: Dec 27, 2013
 *      Author: kazu
 */

#ifndef RTMIDIWRAPPER_H_
#define RTMIDIWRAPPER_H_

#include "../lib/rtmidi/RtMidi.h"
#include "RtMidiException.h"
#include "RtMidiInDevice.h"
#include "RtMidiOutDevice.h"

/**
 * wrapper for the RTMidi library for better event handling
 */
class RtMidiWrapper {


public:

	/** ctor */
	RtMidiWrapper() {
		init();
	}

	/** dtor */
	~RtMidiWrapper() {
		for (RtMidiInDevice* d : devices.in) {delete d;}
		for (RtMidiOutDevice* d : devices.out) {delete d;}
	}

	/** get all available input devices */
	std::vector<RtMidiInDevice*> getInputs() const {
		return devices.in;
	}

	/** get all available output devices */
	std::vector<RtMidiOutDevice*> getOutputs() const {
		return devices.out;
	}


private:

	/** initialize midi input / output */
	void init() {

		// handle for midi-input
		RtMidiIn* midiin = nullptr;

		// open midi-in
		try {midiin = new RtMidiIn();} catch (RtError &error) {
			throw RtMidiException("error while opening midi-in", error);
		}

		// open all devices
		for (unsigned int i = 0; i < midiin->getPortCount(); ++i) {
			devices.in.push_back(new RtMidiInDevice(i));
		}

		delete midiin;


		// handle for midi-output
		RtMidiOut* midiout = nullptr;

		// open midi-out
		try {midiout = new RtMidiOut();} catch (RtError &error) {
			throw RtMidiException("error while opening midi-out", error);
		}

		// open all devices
		for (unsigned int i = 0; i < midiout->getPortCount(); ++i) {
			devices.out.push_back(new RtMidiOutDevice(i));
		}

		delete midiout;

	}


	/** midi in/out devices */
	struct {
		std::vector<RtMidiInDevice*> in;
		std::vector<RtMidiOutDevice*> out;
	} devices;

};



#endif /* RTMIDIWRAPPER_H_ */
