/*
 * SoundSinkNull.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kazu
 */

#ifndef SOUNDSINKHARDWARENULL_H_
#define SOUNDSINKHARDWARENULL_H_

#include "SoundSinkHardware.h"

/**
 * dummy sound sink.
 * simply does nothing at all.
 */
class SoundSinkHardwareNull : public SoundSinkHardware {

public:

	/** ctor */
	SoundSinkHardwareNull() {;}

	/** dtor */
	~SoundSinkHardwareNull() {;}

	void open(AudioFormat fmt) override {
		(void) fmt;
	}

	void push(const Amplitude** channels, const SampleFrame samples) override {
		(void) channels;
		(void) samples;
	}

	void finalize() override {
		;
	}

	std::string getName() override {
		return "Dummy";
	}

	int getLatencyMS() override {
		return 0;
	}

private:

};


#endif /* SOUNDSINKHARDWARENULL_H_ */
