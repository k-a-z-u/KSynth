/*
 * RtMidiOutDevice.h
 *
 *  Created on: Dec 27, 2013
 *      Author: kazu
 */

#ifndef RTMIDIOUTDEVICE_H_
#define RTMIDIOUTDEVICE_H_

#include "RtMidiDevice.h"

class RtMidiOutDevice : public RtMidiDevice {

public:

	/** dtor */
	~RtMidiOutDevice() {
		if (handle) {delete handle; handle = nullptr;}
	}

	RtMidiDeviceType getDeviceType() override {
		return RtMidiDeviceType::OUTPUT;
	}

	/** open this device */
	void open() {

		if (isOpen) {return;}
		isOpen = true;

		// open
		handle->openPort(id);

	}

	void sendEvent(MidiEvent evt) {
		std::vector<unsigned char> vec = evt.asVector();
		handle->sendMessage(&vec);
	}

private:

	friend class RtMidiWrapper;

	/** ctor */
	RtMidiOutDevice(unsigned int idx) : isOpen(false) {

		this->id = idx;

		// open midi-out
		try {handle = new RtMidiOut();} catch (RtError &error) {
			throw RtMidiException("error while opening midi-out", error);
		}

		// fetch name
		try {name = handle->getPortName(idx);} catch ( RtError &error ) {
			throw RtMidiException("error while fetching midi-device's name.", error);
		}

	}

	/** hidden copy ctor */
	RtMidiOutDevice(const RtMidiOutDevice&);

	/** the RtMidi handle */
	RtMidiOut* handle;

	/** check wether the device is currently open */
	bool isOpen;

};



#endif /* RTMIDIOUTDEVICE_H_ */
