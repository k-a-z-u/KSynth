/*
 * MidiBinding.h
 *
 *  Created on: Dec 29, 2013
 *      Author: kazu
 */

#ifndef MIDIBINDING_H_
#define MIDIBINDING_H_

#include "MidiEvent.h"
#include "../misc/SoundBase.h"
#include "rt/RtMidiInDevice.h"

/**
 * glue between midi events and sound devices.
 *
 * specific midi events (e.g control-changes) can be
 * routed to SoundDevice parameters like Synthesizer1's
 * main volume.
 */
struct MidiBinding {

	/** ctor */
	MidiBinding() {;}


	/** source of the binding */
	struct Source {

		/** the name of the midi device to route */
		std::string srcName;

		/** the type of event to route (must be set) */
		MidiEventType type;

		/** the channel number to route (-1 for all channels) */
		int channel;

		/** the first data byte to route (-1 to ignore this byte) */
		int d1;

		/** ctor */
		Source() : type(MidiEventType::CONTROL_CHANGE), channel(-1), d1(-1) {;}

		/** reset the source */
		void reset() {
			srcName = "";
			type = MidiEventType::CONTROL_CHANGE;
			channel = -1;
			d1 = -1;
		}

	} src;


	/** target of the binding */
	struct Target {

		/** the target device */
		SoundBase* dev;

		/** the parameter to change within the target device */
		Param param;

		/** ctor */
		Target() : dev(nullptr), param(0) {;}

		/** reset the destination */
		void reset() {dev = nullptr; param = 0;}

	} dst;

	/**
	 * does this binding match the given MidiDevice + the received MidiEvent?
	 * @param dev the device which received the given event
	 * @param evt the received MidiEvent
	 * @return true on match, else false
	 */
	bool matches(const RtMidiInDevice& dev, const MidiEvent evt) {
		return
				(src.srcName == dev.getName()) &&
				(src.type == evt.getType()) &&
				(src.channel == -1 || src.channel == evt.getChannel()) &&
				(src.d1 == -1 || src.d1 == evt.getData1());

	}

	/** reset the binding */
	void reset() {
		dst.reset();
		src.reset();
	}

};


#endif /* MIDIBINDING_H_ */
