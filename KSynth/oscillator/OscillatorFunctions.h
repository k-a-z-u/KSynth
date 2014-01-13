/*
 * OscillatorFunctions.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kazu
 */

#ifndef OSCILLATORFUNCTIONS_H_
#define OSCILLATORFUNCTIONS_H_

//#define OSC_USE_LUT
#define OSC_LUT_SIZE	(0xFFFF+1)

#include <cstdint>
#include <cmath>
#include <random>
#include "../misc/DataTypes.h"

/**
 * implementation for all oscillator functions like
 * sine, saw, square, triangle, ...
 */
class OscillatorFunction {

public:

//	OscillatorFunction() {;}

//	static Amplitude getNone(unsigned int phase) {
//		(void) phase;
//		return 0;
//	}

//	/** get noise for time and frequency */
//	static Amplitude getNoise(unsigned int phase) {
//		return _getNoise(phase/(float)OSC_LUT_SIZE, 1);
//	}

//	/** get sine for time and frequency */
//	static Amplitude getSine(unsigned int phase) {
//		#ifdef OSC_USE_LUT
//			//unsigned int idx = (int) (t*freq * OSC_LUT_SIZE) % OSC_LUT_SIZE;
//			unsigned int idx = phase & (OSC_LUT_SIZE-1);
//			return lutSine[idx];
//		#else
//			return _getSine(phase/(float)OSC_LUT_SIZE, 1);
//		#endif
//	}

//	/** get triangle for time and frequency */
//	static Amplitude getTriangle(unsigned int phase) {
//		#ifdef OSC_USE_LUT
//			//unsigned int idx = (int) (t*freq * OSC_LUT_SIZE) % OSC_LUT_SIZE;
//			unsigned int idx = phase & (OSC_LUT_SIZE-1);
//			return lutTriangle[idx];
//		#else
//			return _getTriangle(t, freq);
//		#endif
//	}

//	/** get saw for time and frequency */
//	static Amplitude getSaw(unsigned int phase) {
//		#ifdef OSC_USE_LUT
//			//unsigned int idx = (int) (t*freq * OSC_LUT_SIZE) % OSC_LUT_SIZE;
//			unsigned int idx = phase & (OSC_LUT_SIZE-1);
//			return lutSaw[idx];
//		#else
//			return _getSaw(t, freq);
//		#endif
//	}

//	/** get (inverted) saw for time and frequency */
//	static Amplitude getInvSaw(unsigned int phase) {
//		return -getSaw(phase);
//	}

//	/** get square for time and frequency */
//	static Amplitude getSquare(unsigned int phase) {
//		#ifdef OSC_USE_LUT
//			//unsigned int idx = (int) (t*freq * OSC_LUT_SIZE) % OSC_LUT_SIZE;
//			unsigned int idx = phase & (OSC_LUT_SIZE-1);
//			return lutSquare[idx];
//		#else
//			return _getSquare(phase);
//		#endif
//	}

//	/** initialize all look-up-tables */
//	static void initLUTs() {
//		#ifdef OSC_USE_LUT
//			for (unsigned int i = 0; i < OSC_LUT_SIZE; ++i) {lutSine[i] =		_getSine(i / (float) OSC_LUT_SIZE, 1);}
//			for (unsigned int i = 0; i < OSC_LUT_SIZE; ++i) {lutSaw[i] =		_getSaw(i / (float) OSC_LUT_SIZE, 1);}
//			for (unsigned int i = 0; i < OSC_LUT_SIZE; ++i) {lutSquare[i] =		_getSquare(i / (float) OSC_LUT_SIZE, 1);}
//			for (unsigned int i = 0; i < OSC_LUT_SIZE; ++i) {lutTriangle[i] =	_getTriangle(i / (float) OSC_LUT_SIZE, 1);}
//		#endif
//	}

private:

//	inline static Amplitude _getSquare(Time t, Frequency freq)		{return 1.0 - 2.0 * ((int)(2*t*freq) & 1);}
//	inline static Amplitude _getSine(Time t, Frequency freq)		{return sin(K_PI2 * t * freq);}
//	inline static Amplitude _getSaw(Time t, Frequency freq)			{return ((int16_t)(t*freq*0xFFFF)) / (double) 0x7FFF;}
//	inline static Amplitude _getTriangle(Time t, Frequency freq)	{
//		float x = fmodf(t*freq, 1.0);
//		if (x < 0.25) {return (4*x);}
//		else if (x < 0.5) {return 2.0 - (4*x);}
//		else if (x < 0.75) {return (2.0 - 4*x);}
//		else return -4.0 + 4*x;
//	}
//	inline static Amplitude _getNoise(Time t, Frequency freq)		{(void) t; (void) freq; return (rand() / (double) RAND_MAX - 0.5) / 4;}


#ifdef OSC_USE_LUT
	static Amplitude lutSine[OSC_LUT_SIZE];
	static Amplitude lutSaw[OSC_LUT_SIZE];
	static Amplitude lutSquare[OSC_LUT_SIZE];
	static Amplitude lutTriangle[OSC_LUT_SIZE];
#endif

};




#endif /* OSCILLATORFUNCTIONS_H_ */
