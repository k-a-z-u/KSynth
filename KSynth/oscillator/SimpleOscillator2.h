/*
 * SimpleOscillator2.h
 *
 *  Created on: Jan 1, 2014
 *      Author: kazu
 */

#ifndef SIMPLEOSCILLATOR2_H_
#define SIMPLEOSCILLATOR2_H_

#include "../misc/DataTypes.h"
#include <cmath>


/**
 * the size (number of entries) to use for look-up-tables
 * this should be a power of 2 as those values allow bit-masking (&)
 * instead of modulo (%) for region clamping.
 */
#define SO_LUT_SIZE		65536


/**
 * all supported modes for the simple oscillator
 */
enum class SimpleOscillator2Mode {
	NONE,
	SINE,
	SQUARE,
	SQUARE_NO_ALIAS,
	SAW,
	SAW_NO_ALIAS,
	INV_SAW,
	INV_SAW_NO_ALIAS,
	TRIANGLE,
	NOISE,

	_END,
};


/**
 * provides some generator functions converting phase [0:1] to an amplitude.
 */
struct SimpleOscillator2Func {

	/** slightly smoothes the given funtion to reduce aliasing effects */
	static Amplitude smooth( Amplitude (*generator) (float phase), float phase ) {
		Amplitude ret = 0;
		for (int i = -5; i <= 5; ++i) {ret += generator(phase + 1.0f + float(i) * 0.01f);}
		return ret / 10;
	}


	/** dummy function. returns 0 */
	static Amplitude null(const float phase)		{(void) phase; return (Amplitude) 0;}


	/** a sine function */
	static Amplitude sine(const float phase)		{return Amplitude( std::sin(K_PI2 * phase) );}


	/** creates a raw square [-1/+1] only -> aliasing effects! */
	static Amplitude square(const float phase)		{return (Amplitude)  1.0 - (Amplitude) 2.0 * (std::fmod(phase,1.0) > (Amplitude)0.5);}

	/** slightly smothed square -> less aliasing */
	static Amplitude squareNoAlias(const float phase) {return smooth(&square, phase);}


	/** create a saw-tooth wave */
	static Amplitude saw(const float phase)			{return (Amplitude) -1.0 + (Amplitude) 2.0 * std::fmod(phase+0.5f, 1.0f);}

	/** create a smothed sawtooth -> less aliasing */
	static Amplitude sawNoAlias(const float phase)	{return smooth(&saw, phase);}


	/** inverted sawtooth */
	static Amplitude invSaw(const float phase)	{return -saw(phase);}

	/** create a smothed inverted sawtooth -> less aliasing */
	static Amplitude invSawNoAlias(const float phase)	{return smooth(&invSaw, phase);}


};


/**
 * provides look-up-tables for the oscillator.
 * those tables will be instantiated only once per function
 */
class SimpleOscillator2LUT {

public:

	/** singleton access */
	static SimpleOscillator2LUT& get() {
		static SimpleOscillator2LUT lut;
		return lut;
	}

	/** build the LUT for the given mode (if not already built) */
	Amplitude* buildLUT(SimpleOscillator2Mode mode,  Amplitude (*generator) (float phase)) {

		int idx = int(mode);

		// LUT already built? -> return
		if (lut[idx]) {return lut[idx];}

		// allocate memory
		lut[idx] = new Amplitude[SO_LUT_SIZE]; //(Amplitude*) malloc(sizeof(Amplitude) * SO_LUT_SIZE);

		// fill
		for (unsigned int i = 0; i < SO_LUT_SIZE; ++i) {
			lut[idx][i] = generator( float(i) / float(SO_LUT_SIZE) );
		}

		// return
		return lut[idx];

	}

private:

	/** ctor */
	SimpleOscillator2LUT() : lut() {
		for (int i = 0; i < int(SimpleOscillator2Mode::_END); ++i) {
			lut[i] = nullptr;
		}
	}

	/** hidden copy ctor */
	SimpleOscillator2LUT(const SimpleOscillator2LUT& o);

	/** hidden assignment operator */
	SimpleOscillator2LUT& operator = (const SimpleOscillator2LUT& o);


	/** dtor */
	~SimpleOscillator2LUT() {
		for (int i = 0; i < int(SimpleOscillator2Mode::_END); ++i) {
			delete lut[i];
			lut[i] = nullptr;
		}
	}


	/** LUTs are stored here */
	Amplitude* lut[ int(SimpleOscillator2Mode::_END) ];

};



/**
 * provides several access-methods to sound generating functions above
 */
class SimpleOscillator2 {

public:

	/** ctor */
	SimpleOscillator2() : lut(nullptr) {
		setMode(SimpleOscillator2Mode::NONE);
	}

	/** dtor */
	~SimpleOscillator2() {

	}

	void setMode(SimpleOscillator2Mode mode) {

		this->mode = mode;

		switch (mode) {
			case SimpleOscillator2Mode::NONE:				generator = &SimpleOscillator2Func::null;			break;
			case SimpleOscillator2Mode::SINE:				generator = &SimpleOscillator2Func::sine;			break;
			case SimpleOscillator2Mode::SQUARE:				generator = &SimpleOscillator2Func::square;			break;
			case SimpleOscillator2Mode::SQUARE_NO_ALIAS:	generator = &SimpleOscillator2Func::squareNoAlias;	break;

			case SimpleOscillator2Mode::SAW:				generator = &SimpleOscillator2Func::saw;			break;
			case SimpleOscillator2Mode::SAW_NO_ALIAS:		generator = &SimpleOscillator2Func::sawNoAlias;		break;

			case SimpleOscillator2Mode::INV_SAW:			generator = &SimpleOscillator2Func::invSaw;			break;
			case SimpleOscillator2Mode::INV_SAW_NO_ALIAS:	generator = &SimpleOscillator2Func::invSawNoAlias;	break;

			case SimpleOscillator2Mode::TRIANGLE:			generator = &SimpleOscillator2Func::null;			break;
			case SimpleOscillator2Mode::NOISE:				generator = &SimpleOscillator2Func::null;			break;
			case SimpleOscillator2Mode::_END:				break;
		}

		lut = SimpleOscillator2LUT::get().buildLUT(mode, generator);

	}


	/** get the current oscillation mode */
	SimpleOscillator2Mode getMode() const {
		return this->mode;
	}

	Amplitude get(float phase) const {
		return generator(phase);
	}

	Amplitude getLUT(float phase) const {
		unsigned int idx = (unsigned int) (phase * SO_LUT_SIZE) % SO_LUT_SIZE;
		return lut[idx];
	}

	float getLUTmultiplier(SampleRate sRate) const {
		return SO_LUT_SIZE / (float) sRate;
	}

	Amplitude getLUTint(unsigned int phase) const {
		unsigned int idx = phase % SO_LUT_SIZE;
		return lut[idx];
	}

	unsigned int getLUTsize() const {
		return SO_LUT_SIZE;
	}

	Amplitude getLUTinterpolated(float phase) const {
		float f1 = phase * SO_LUT_SIZE;
		float blend2 = f1 - std::floor(f1);
		float blend1 = 1.0f - blend2;
		unsigned int idx1 = (unsigned int) f1 % SO_LUT_SIZE;
		unsigned int idx2 = idx1 + 1;
		return (lut[idx1] * blend1) + (lut[idx2] * blend2);
	}


private:

	Amplitude (*generator) (float phase);

	/** the mode to use */
	SimpleOscillator2Mode mode;

	/** look-up-table (created within SimpleOscillator2LUT) */
	Amplitude* lut;

};








#endif /* SIMPLEOSCILLATOR2_H_ */
