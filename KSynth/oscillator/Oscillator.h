/*
 * Oscillator.h
 *
 *  Created on: Oct 23, 2013
 *      Author: kazu
 */

#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#include "../misc/DataTypes.h"

/**
 * interface for all oscillators
 */
class Oscillator {

public:

	/** dtor */
	virtual ~Oscillator() {;}

	/** get the oscillation value (amplitude) for the given time */
	virtual Amplitude get(Time t) const = 0;

	/** simply get the amplitude of the underlying function for the given phase */
	virtual Amplitude getByPhase(unsigned int phase) const = 0;

};


#endif /* OSCILLATOR_H_ */
