/*
 * OscillatorFunction.h
 *
 *  Created on: Jan 1, 2014
 *      Author: kazu
 */

#ifndef OSCILLATORFUNCTION_H_
#define OSCILLATORFUNCTION_H_

/**
 * oscillator function using functors
 */
template <typename functor> class OscillatorFunction {

public:

	/** get amplitude for the given phase */
	Amplitude get(float phase) {
		return functor(phase);
	}

};



#endif /* OSCILLATORFUNCTION_H_ */
