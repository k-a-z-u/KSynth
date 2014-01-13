/*
 * MasterTarget.h
 *
 *  Created on: Jan 7, 2014
 *      Author: kazu
 */

#ifndef MASTERTARGET_H_
#define MASTERTARGET_H_

#include "../misc/SoundBase.h"
#include "../misc/VUCalc.h"

/**
 * all syntesizers / fx plugins will stream into this
 * (final) master target.
 */
class MasterTarget : public SoundBase {

public:

	/** ctor */
	MasterTarget() : SoundBase(2, 2) {

	}

	std::string getProductString() const override {
		return "MasterTarget";
	}

	void process(Amplitude** inputs, Amplitude** outputs) override {

		if (!inputs) {return;}

		// clamp left output
		if (inputs[0]) {
			for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
				outputs[0][i] = clamp1(inputs[0][i]);
			}
			vuLeft.get(outputs[0], getSamplesPerProcess());
		}

		// clamp right output
		if (inputs[1]) {
			for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
				outputs[1][i] = clamp1(inputs[1][i]);
			}
			vuRight.get(outputs[1], getSamplesPerProcess());
		}

	}

	unsigned int getNumParameters() const override {
		return 0;
	}

	bool getInputProperties(unsigned int idx, PinProperties* properties) override {
		switch(idx) {
		case 0: properties->name = "input (left)"; return true;
		case 1: properties->name = "input (right)"; return true;
		}
		return false;
	}

	bool getOutputProperties(unsigned int idx, PinProperties* properties) override {
		(void) idx;
		(void) properties;
		return false;
	}


protected:

	Volume getVULeft() {return vuLeft.getMax();}
	Volume getVURight() {return vuRight.getMax();}


private:

	VUCalc vuLeft;
	VUCalc vuRight;
};


#endif /* MASTERTARGET_H_ */
