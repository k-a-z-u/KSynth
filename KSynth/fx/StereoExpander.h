#ifndef STEREOEXPANDER_H
#define STEREOEXPANDER_H

#include "StereoExpanderGate.h"
#include "../misc/SoundProcessor.h"

class StereoExpander : public SoundProcessor {

public:

	/** ctor */
	StereoExpander() : SoundProcessor(2,2) {
		gate.setWidth(0.4f);
	}

	void process(Amplitude** inputs, Amplitude** outputs) override {

		if (gate.getWidth() < 0.01) {
			outputs[0] = inputs[0];
			outputs[1] = inputs[1];
		} else {
			for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
				gate.process(inputs[0][i], inputs[1][i], outputs[0][i], outputs[1][i]);
			}
		}

	}

	virtual unsigned int getNumParameters() const override {
		return 1;
	}

	virtual std::string getProductString() const override {
		return "StereoExpander";
	}

private:

	StereoExpanderGate gate;

};

#endif // STEREOEXPANDER_H
