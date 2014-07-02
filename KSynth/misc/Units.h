/*
 * Units.h
 *
 *  Created on: Jan 2, 2014
 *      Author: kazu
 */

#ifndef UNITS_H_
#define UNITS_H_

#include "DataTypes.h"
#include <iostream>
/**
 * provides various conversions / scalings
 * between different units / scales
 */
class Units {

public:


	/** convert value [0;1] to a gain (multiplicator) value */
	static Volume valueToGain(const float v) {
		return v * 1.27f;
	}

	/** convert gain to a float value [0;1] */
	static float gainToValue(const Volume gain) {
		return gain / 1.27f;
	}


	/** convert [0;1] to biquad frequency region of [0;0.5] using a non-linear scale */
	static float paramToLogFreqFact( const ParamValue pv ) {
		return std::pow( pv, 3.0f ) * 0.5f;
	}

	/** convert non-linear freqFact [0;0.5] back to param region of [0;1] */
	static ParamValue logFreqFactToParam( const float logFreqFact ) {
		return std::pow( logFreqFact * 2.0f, 0.33333f );
	}


	/** convert amplitude [-1;+1] to dB value */
	static float amplitudeToDB( const Amplitude a ) {
		return float(10.0 * log10(a));
	}

	/** convert dB value to amplitude */
	static Amplitude dbToAmplitude( const float db ) {
		return Amplitude(pow(10, db / 10.0));
	}



	/** convert the given value [0;1] to a frequency [~0;maxHz] */
	static Frequency floatToFreq( const float v, const unsigned int maxHz = 22050 ) {
		return Frequency( pow( 1000.0f, v) * ((float)maxHz / 1000.0f) );
	}

	/** convert the given frequency [0;maxHz] back to a float value [0;1] */
	static float freqToFloat( const Frequency freq, const unsigned int maxHz = 22050 ) {
		return float( log( freq / ((float)maxHz / 1000.0f) ) / log(1000.0f) );
	}

};



#endif /* UNITS_H_ */
