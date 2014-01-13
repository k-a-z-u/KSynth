/*
 * Units.h
 *
 *  Created on: Jan 2, 2014
 *      Author: kazu
 */

#ifndef UNITS_H_
#define UNITS_H_

#include "DataTypes.h"

/**
 * provides various conversions / scalings
 * between different units / scales
 */
class Units {

public:


	/** convert value [0;1] to a gain (multiplicator) value */
	static Volume valueToGain(float v) {
		return v * 1.27f;
	}

	/** convert gain to a float value [0;1] */
	static float gainToValue(Volume gain) {
		return gain / 1.27f;
	}


	/** convert amplitude [-1;+1] to dB value */
	static float amplitudeToDB(Amplitude a) {
		return float(10.0 * log10(a));
	}

	/** convert dB value to amplitude */
	static Amplitude dbToAmplitude(float db) {
		return Amplitude(pow(10, db / 10.0));
	}



	/** convert the given value [0;1] to a frequency [~0;maxHz] */
	static Frequency floatToFreq(float v, unsigned int maxHz = 22050) {
		return Frequency( pow( 1000.0f, v) * ((float)maxHz / 1000.0f) );
	}

	/** convert the given frequency [0;maxHz] back to a float value [0;1] */
	static float freqToFloat(Frequency freq, unsigned int maxHz = 22050) {
		return float( log( freq / ((float)maxHz / 1000.0f) ) / log(1000.0f) );
	}

};



#endif /* UNITS_H_ */
