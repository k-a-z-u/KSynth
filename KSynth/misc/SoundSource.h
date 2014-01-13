/*
 * SoundSource.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef SOUNDSOURCE_H_
#define SOUNDSOURCE_H_

#include "DataTypes.h"
#include "Events.h"
#include "SoundBase.h"

/**
 * interface for all sound providing elements
 */
class SoundSource : public SoundBase {

public:

	/** ctor */
	SoundSource(unsigned int numOutputs) : SoundBase(0, numOutputs) {;}

	/** dtor */
	virtual ~SoundSource() {;}

	/** get amplitude of the source for the given time */
	//virtual AudioData get(Time t) = 0;


	Time getTimePerSampleFrame() {return 1.0f / (Time) getSampleRate();}




};

/**
 * dummy sound source
 * always returns 0
 * can be used to reduce branching
 */
struct SoundSourceDummy : public SoundSource {
	SoundSourceDummy() : SoundSource(2) {;}
	//AudioData get(Time t) {(void) t; return AudioData();}
	void process(Amplitude** input, Amplitude** output) override {
		(void) input;
		(void) output;
	}
	unsigned int getNumParameters() const override {return 0;}
	std::string getProductString() const {return "DUMMY";}
	void setParameterName(Param p, const std::string& name) {
		(void) p;
		(void) name;
	}
};
static SoundSourceDummy soundSourceDummy;


#endif /* SOUNDSOURCE_H_ */
