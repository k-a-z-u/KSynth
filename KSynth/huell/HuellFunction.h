/*
 * HuellFunction.h
 *
 *  Created on: Oct 23, 2013
 *      Author: kazu
 */

#ifndef HUELLFUNCTION_H_
#define HUELLFUNCTION_H_

#include "../misc/DataTypes.h"
#include "../notes/NoteDescription.h"


/**
 * interface for all huell functions
 */
class HuellFunction {

public:

	/** dtor */
	virtual ~HuellFunction() {;}

	/** get amplitude at time t */
	virtual Amplitude get(const NoteDescription& nd, Time age) = 0;

};


#endif /* HUELLFUNCTION_H_ */
