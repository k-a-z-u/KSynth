/*
 * AudioFormat.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef AUDIOFORMAT_H_
#define AUDIOFORMAT_H_

#include "DataTypes.h"

struct AudioFormat {

	/** the number of channels to use */
	unsigned int numChannels;

	/** the sample-rate in Hz */
	unsigned int sampleRate;

	/** the number of bits to use */
	unsigned int bits;

	/** empty ctor */
	AudioFormat() : numChannels(1), sampleRate(48000), bits(16) {;}

	AudioFormat(unsigned int channels, SampleRate sampleRate, unsigned int bits) :
		numChannels(channels), sampleRate(sampleRate), bits(bits) {;}

};



#endif /* AUDIOFORMAT_H_ */
