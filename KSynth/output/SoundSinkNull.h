/*
 * SoundSinkNull.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kazu
 */

#ifndef SOUNDSINKNULL_H_
#define SOUNDSINKNULL_H_

#include "SoundSink.h"

/**
 * dummy sound sink.
 * simply does nothing at all.
 */
class SoundSinkNull : public SoundSink {

public:

	/** ctor */
	SoundSinkNull() {;}

	/** dtor */
	~SoundSinkNull() {;}

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


#endif /* SOUNDSINKNULL_H_ */
