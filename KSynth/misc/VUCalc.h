/*
 * VUCalc.h
 *
 *  Created on: Dec 21, 2013
 *      Author: kazu
 */

#ifndef VUCALC_H_
#define VUCALC_H_


/**
 * provides a simple VU calculation
 */
class VUCalc {

public:

	VUCalc() {
		reset();
	}

	/** reset the current maximum */
	void reset() {
		max = 0;
	}

	/** get the current maximum */
	Volume getMax() {
		return max;
	}

	/** add a new value */
	void add(Amplitude a) {
		max = (max > a) ? (max) : (a);
	}

	/** get VU from given amplitude array */
	void get(Amplitude* arr, unsigned int len) {
		reset();
		for (unsigned int i = 0; i < len; ++i) {
			add(arr[i]);
		}
	}

private:

	/** the current max */
	Volume max;

};


#endif /* VUCALC_H_ */
