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
#include "RtMidiEventListener.h"

#include <algorithm>
#include <vector>
#include <memory>


/**
 * wrapper for the RTMidi library.
 *
 * uses object oriented access to midi devices and
 * provides a simple event listening.
 *
 * to start things up, call init(), which might throw an exception
 * if anything goes wrong. if anything went wrong during init(), this
 * class behaves like a convenient dummy and does not report any midi
 * device at all.
 *
 */
class RtMidiWrapper : RtMidiEventListener {


public:

	/** ctor */
	RtMidiWrapper() {
		;
	}

	/** dtor */
	~RtMidiWrapper() {
		;
	}

	/** get all available input devices */
	const std::vector< std::unique_ptr<RtMidiInDevice> >& getInputs() const {
		return devices.in;
	}

	/** get all available output devices */
	const std::vector< std::unique_ptr<RtMidiOutDevice> >& getOutputs() const {
		return devices.out;
	}

	/**
	 * get input device by its name
	 * @param name the device name to look-up
	 * @return the device or nullptr if none matched
	 */
	RtMidiInDevice* getInput(const std::string& name) {
		for ( auto& in : devices.in ) {
			if (in->getName() == name) {return in.get();}
		}
		return nullptr;
	}

	/**
	 * get output device by its name
	 * @param name the device name to look-up
	 * @return the device or nullptr if none matched
	 */
	RtMidiOutDevice* getOutput(const std::string& name) {
		for ( auto& out : devices.out) {
			if (out->getName() == name) {return out.get();}
		}
		return nullptr;
	}

	/**
	 * add a new event listener to ALL devices.
	 * whenever a midi event on any of the attached devices is recognized,
	 * the listener will be informed.
	 * @param l the listener to add
	 */
	void addListener(RtMidiEventListener* l) {
		listeners.inEvents.push_back(l);
	}

	/**
	 * remove an exisitring midi event listeners.
	 * @param l the listener to remove
	 */
	void removeListener(RtMidiEventListener* l) {
		auto match = [l] (const RtMidiEventListener* other) {return l == other;};
		listeners.inEvents.erase( std::remove_if(listeners.inEvents.begin(), listeners.inEvents.end(), match), listeners.inEvents.end());
	}

	/**
	 * initialize midi input/output.
	 * this will also load a list of all currently attached
	 * input and output devices.
	 */
	void init() {

		// handle for midi-input
		RtMidiIn* midiin = nullptr;

		// open midi-in
		try {midiin = new RtMidiIn();} catch (RtError &error) {
			throw RtMidiException("error while opening midi-in", error);
		}

		// open all devices
		devices.in.clear();
		for (unsigned int i = 0; i < midiin->getPortCount(); ++i) {
			RtMidiInDevice* dev = new RtMidiInDevice(i);
			std::string name = dev->getName();
			std::string end = name.substr(name.length()-2);
			if (end[0] != ':' || end[1] == '0') {
				std::unique_ptr<RtMidiInDevice> ptr(dev);
				ptr->addListener(this);
				devices.in.push_back( std::move(ptr) );
			}
		}

		delete midiin;


		// handle for midi-output
		RtMidiOut* midiout = nullptr;

		// open midi-out
		try {midiout = new RtMidiOut();} catch (RtError &error) {
			throw RtMidiException("error while opening midi-out", error);
		}

		// open all devices
		devices.out.clear();
		for (unsigned int i = 0; i < midiout->getPortCount(); ++i) {
			std::unique_ptr<RtMidiOutDevice> ptr(new RtMidiOutDevice(i));
			devices.out.push_back( std::move(ptr) );
		}

		delete midiout;

	}


private:

	/**
	 * register myself as listener for every attached device.
	 * any received event is then sent to all of my listeners.
	 * (this should be a better approach than attaching each listener
	 * to each individual device)
	 */
	virtual void onMidiEvent(RtMidiInDevice& src, MidiEvent evt) override {
		for (RtMidiEventListener* l : listeners.inEvents) {
			l->onMidiEvent(src, evt);
		}
	}

	/** midi in/out devices */
	struct {
		std::vector< std::unique_ptr<RtMidiInDevice> > in;
		std::vector< std::unique_ptr<RtMidiOutDevice> > out;
	} devices;

	/** listeners */
	struct {

		/** listen for incoming midi events among all devices */
		std::vector<RtMidiEventListener*> inEvents;

	} listeners;

};



#endif /* RTMIDIWRAPPER_H_ */
