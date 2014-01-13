/*
 * AGC.h
 *
 *  Created on: Dec 22, 2013
 *      Author: kazu
 */

#ifndef AGC_H_
#define AGC_H_

#include <vector>

/**
 * auto gain analyzer.
 *
 * takes an array of amplitudes as input, get the maximum of those values
 * and keeps a history of among several subsequent calls to maintain
 * long-time gain maxima.
 *
 * the size of the history depends on the used sample-rate and number
 * of samples per call. using the history also low frequencies like
 * 20Hz can be auto gain-controlled
 *
 */
class AGC {

public:

	/**
	 * ctor
	 * @param rate the used sample rate. the buffer size depends on this param
	 * @param bs the number of samples per call. the buffer size depends on this param
	 */
	AGC(SampleRate rate, SampleFrame bs) : curIdx(0), curMax(0) {
		history.resize(rate / 40 / bs);
		this->size = (unsigned int) history.size();
	}

	void add(Amplitude* data, unsigned int numEntries) {

		// add all entries to current slot
		history[curIdx] = 0;
		for (unsigned int i = 0; i < numEntries; ++i) {
			if (data[i] == data[i] && data[i] > history[curIdx]) {history[curIdx] = data[i];}
		}

		// switch to next slot
		curIdx = (curIdx+1) % size;

		// calculate new maximum
		curMax = 0;
		for (unsigned int i = 0; i < size; ++i) {
			if (history[i] > curMax) {curMax = history[i];}
		}

//		// use average value as maximum
//		curMax = 0;
//		for (unsigned int i = 0; i < size; ++i) {curMax += history[i];}
//		curMax /= size;

	}

	Amplitude getMax() {
		return curMax;
	}

private:

	/** the current history index */
	unsigned int curIdx;

	/** the current gain max among all history entries */
	Amplitude curMax;

	/** the number of entries within the history (cached) */
	unsigned int size;

	/** the history buffer */
	std::vector<Amplitude> history;

};


#endif /* AGC_H_ */
