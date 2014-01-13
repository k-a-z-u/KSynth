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


enum class SimpleOscillator2Mode {
	NONE,
	SINE,
	SQUARE,
	SAW,
	INV_SAW,
	TRIANGLE,
	NOISE,

	_END,
};


struct SimpleOscillator2Func {

	static Amplitude null(const float phase)	{(void) phase; return (Amplitude) 0;}

	static Amplitude sine(const float phase)	{return Amplitude( std::sin(K_PI2 * phase) );}

	static Amplitude square(const float phase)	{return (Amplitude)  1.0 - (Amplitude) 2.0 * (std::fmod(phase,1.0) > (Amplitude)0.5);}

	static Amplitude saw(const float phase)		{return (Amplitude) -1.0 + (Amplitude) 2.0 * std::fmod(phase+0.5f, 1.0f);}

	static Amplitude invSaw(const float phase)	{return -saw(phase);}


};


#define SO_LUT_SIZE		65536

class SimpleOscillator2 {

public:

	/** ctor */
	SimpleOscillator2() : lut(nullptr) {
		lut = (Amplitude*) malloc(sizeof(Amplitude) * SO_LUT_SIZE);
		setMode(SimpleOscillator2Mode::NONE);
	}

	/** dtor */
	~SimpleOscillator2() {
		free(lut);
	}

	void setMode(SimpleOscillator2Mode mode) {
		this->mode = mode;
		switch (mode) {
		case SimpleOscillator2Mode::NONE:		generator = &SimpleOscillator2Func::null;		break;
		case SimpleOscillator2Mode::SINE:		generator = &SimpleOscillator2Func::sine;		break;
		case SimpleOscillator2Mode::SQUARE:		generator = &SimpleOscillator2Func::square;		break;
		case SimpleOscillator2Mode::SAW:		generator = &SimpleOscillator2Func::saw;		break;
		case SimpleOscillator2Mode::INV_SAW:	generator = &SimpleOscillator2Func::invSaw;		break;
		case SimpleOscillator2Mode::TRIANGLE:	generator = &SimpleOscillator2Func::null;		break;
		case SimpleOscillator2Mode::NOISE:		generator = &SimpleOscillator2Func::null;		break;
		case SimpleOscillator2Mode::_END:		break;
		}
		buildLUT();
	}

	/** get the current oscillation mode */
	SimpleOscillator2Mode getMode() const {
		return this->mode;
	}

	void buildLUT() {
		for (unsigned int idx = 0; idx < SO_LUT_SIZE; ++idx) {
			lut[idx] = generator( float(idx) / float(SO_LUT_SIZE));
		}
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

	Amplitude* lut;

};





#endif /* SIMPLEOSCILLATOR2_H_ */
