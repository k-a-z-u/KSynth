/*
 * SoundProcessor.h
 *
 *  Created on: Dec 6, 2013
 *      Author: kazu
 */

#ifndef SOUNDPROCESSOR_H_
#define SOUNDPROCESSOR_H_

#include "SoundSource.h"

/**
 * process audio data:
 * take input, modify, and provide new output
 */
class SoundProcessor : public SoundBase {

public:

	/** ctor */
	SoundProcessor(const unsigned int numInputs, const unsigned int numOutputs) :
		SoundBase(numInputs, numOutputs) {;}

	virtual ~SoundProcessor() {;}




};


#endif /* SOUNDPROCESSOR_H_ */
