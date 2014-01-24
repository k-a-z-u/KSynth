/*
 * RtMidiInDevice.h
 *
 *  Created on: Dec 27, 2013
 *      Author: kazu
 */

#ifndef RTMIDIINDEVICE_H_
#define RTMIDIINDEVICE_H_

#include <algorithm>
#include <vector>

#include "RtMidiException.h"
#include "RtMidiEventListener.h"
#include "RtMidiDevice.h"
#include "rtmidi/RtMidi.h"


class RtMidiInDevice : public RtMidiDevice {

public:

	/** dtor */
	~RtMidiInDevice() {
		if (handle) {delete handle; handle = nullptr;}
	}

	RtMidiDeviceType getDeviceType() override {
		return RtMidiDeviceType::INPUT;
	}

	/** bind a new event listener to this device */
	void addListener(RtMidiEventListener* l) {
		listeners.push_back(l);
		open();
	}

	/** remove an existring event listener */
	void removeListener(RtMidiEventListener* l) {
		auto match = [l] (const RtMidiEventListener* other) {return l == other;};
		listeners.erase( std::remove_if(listeners.begin(), listeners.end(), match), listeners.end());
	}


private:

	friend class RtMidiWrapper;

	/** ctor */
	RtMidiInDevice(unsigned int idx) : isOpen(false) {

		this->id = idx;

		// open midi-in
		try {handle = new RtMidiIn();} catch (RtError &error) {
			throw RtMidiException("error while opening midi-in", error);
		}

		// fetch name
		try {name = handle->getPortName(idx);} catch ( RtError &error ) {
			throw RtMidiException("error while fetching midi-device's name.", error);
		}

		// register callback
		handle->setCallback( &RtMidiInDevice::staticCallback, (void*) this );

	}

	/** open this device and bind to events */
	void open() {

		if (isOpen) {return;}
		isOpen = true;

		// open
		handle->openPort(id);

	}

	/** the callback for every midi event */
	void callback(MidiEvent evt) {
		std::cout << evt.asString() << std::endl;
		for (RtMidiEventListener* l : listeners) {
			l->onMidiEvent(*this, evt);
		}
	}

	/** the static callback for every midi event */
	static void staticCallback( double deltatime, std::vector< unsigned char >* message, void* userData ) {
		RtMidiInDevice* me = (RtMidiInDevice*) userData;
		MidiEvent evt( (uint32_t)(deltatime * 1000), (int)(message->at(0)), (int)(message->at(1)), (int)(message->at(2)));
		me->callback(evt);
	}

	/** the RtMidi handle */
	RtMidiIn* handle;

	/** is this device currently open? */
	bool isOpen;

	/** all event listeners */
	std::vector<RtMidiEventListener*> listeners;

};



#endif /* RTMIDIINDEVICE_H_ */
