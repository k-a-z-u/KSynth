/*
 * SimpleOscillator.h
 *
 *  Created on: Oct 23, 2013
 *      Author: kazu
 */

#ifndef SIMPLEOSCILLATOR_H_
#define SIMPLEOSCILLATOR_H_

#include "Oscillator.h"
#include <cmath>
#include "OscillatorFunctions.h"


enum SimpleOscillatorMode {
	NONE,
	SINE,
	SQUARE,
	SAW,
	INV_SAW,
	TRIANGLE,
	NOISE,

	_END,
};

/**
 * a simple, basic oscillator
 */
class SimpleOscillator : public Oscillator {

public:

	/** ctor */
	SimpleOscillator() {
		setMode(SimpleOscillatorMode::NONE);
	}

	/** dtor */
	~SimpleOscillator() {;}

	void setMode(SimpleOscillatorMode mode) {
		this->mode = mode;
		switch (mode) {
//		case NONE:			generator = &OscillatorFunction::getNone; break;
//		case SINE:			generator = &OscillatorFunction::getSine; break;
//		case SAW:			generator = &OscillatorFunction::getSaw; break;
//		case INV_SAW:		generator = &OscillatorFunction::getInvSaw; break;
//		case SQUARE:		generator = &OscillatorFunction::getSquare; break;
//		case NOISE:			generator = &OscillatorFunction::getNoise; break;
//		case TRIANGLE:		generator = &OscillatorFunction::getTriangle; break;
//		case _END:			break;
		}
	}

	SimpleOscillatorMode getMode() const {return mode;}

	Amplitude getByPhase(unsigned int phase) const override {
		return generator(phase);
	}

	Amplitude get(Time t) const override {
		return generator(t*freq*OSC_LUT_SIZE);
	}

	Amplitude getFrame(SampleFrame frm) const {
		return generator(frm/48000.0f*freq*OSC_LUT_SIZE);
	}

	/** set the frequency for this oscillator */
	void setFrequency(Frequency hz) {
		this->freq = hz;
	}

private:

	/** the frequency to use */
	Frequency freq = 0;

	/** function-pointer for the generator. phase between [0;65535] */
	Amplitude (*generator) (unsigned int phase);

	/** the currently selected mode */
	SimpleOscillatorMode mode;

};


#endif /* SIMPLEOSCILLATOR_H_ */
