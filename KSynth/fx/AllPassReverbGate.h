#ifndef ALLPASSREVERBGATE_H
#define ALLPASSREVERBGATE_H

#include "../misc/DataTypes.h"
#include "DelayBufferFixed.h"

/**
 * represents a very simple (but efficient) reverb
 * gate using a modified comb filter as an all-pass.
 *
 * http://www.earlevel.com/main/1997/01/19/a-bit-about-reverb/
 */
template <typename type, int maxDelay> class AllPassReverbGate {

public:

	/** ctor */
	AllPassReverbGate() : delaySize(0), posGain(0), negGain(0), enabled(true) {
		setDelay(0);
		setPositiveGain(0);
		setNegativeGain(0);
	}

	/** set the strength of the delay to apply [0:1]. 0 will DISABLE the gate! */
	void setDelay(const float val) {

		// as we read the buffer before writing to it, we add one dummy entry
		delaySize = (unsigned int) ( (float) maxDelay * val);
		delayBuffer.setDelay(delaySize + 1);

		// delay(0) will disable the control
		enabled = (val > 0.00001f);

	}

	/** is the gate enabled or disabled? */
	bool isEnabled() const {return enabled;}

	/** get the strenfth of the delay to apply [0:1] */
	float getDelay() const { return (float) delaySize / (float) maxDelay; }


	/** set the positive gain value [0:1] for the delay */
	void setPositiveGain(const float gain) {this->posGain = gain;}

	/** get the positive gain value [0:1] for the delay */
	ParamValue getPositiveGain() const {return this->posGain;}


	/** set the negative gain value [0:1] for the input data */
	void setNegativeGain(const float gain) {this->negGain = -gain;}

	/** get the negative gain value [0:1] for the input data */
	ParamValue getNegativeGain() const {return -this->negGain;}


	/** this will reset the gate by zeroing the delay buffer */
	void reset() { delayBuffer.reset(); }

	/** process the given input sample */
	type process(const type in) {

		// skip?
		if (!enabled) {return in;}


		// get negative version
		type negIn = in * negGain;

		// get delayed version
		type delayed = delayBuffer.pop();

		// combined delayed/negative input version
		type res = delayed + negIn;

		// combine result + original input for the delay buffer
		type newIn = in + (res * posGain);

		// append to delay buffer
		delayBuffer.push(newIn);

		// done
		return res;

	}


private:

	/** a delay buffer to apply the audio delay */
	DelayBufferFixed<type, maxDelay> delayBuffer;

	/** the number of entries from the ringbuffer to use as delay */
	unsigned int delaySize;

	/** positive gain for the delay */
	float posGain;

	/** negative gain for the input data */
	float negGain;

	/** enable/disable the gate */
	bool enabled;


};


#endif // ALLPASSREVERBGATE_H
