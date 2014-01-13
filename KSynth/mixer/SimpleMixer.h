/*
 * SimpleMixer.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef SIMPLEMIXER_H_
#define SIMPLEMIXER_H_

#include "../misc/SoundProcessor.h"
#include "../misc/DataTypes.h"
#include "../misc/Functions.h"
#include "../misc/VUCalc.h"
#include "../misc/Units.h"

#include "../fx/BiquadFilter.h"


#define SM_SLOTS	9

/** enum for each configurable parameter of the SimpleMixer */
enum class SimpleMixerParams {

	MASTER_VOLUME,

	SLOT1_VOLUME,
	SLOT1_LOW_PASS,
	SLOT1_HIGH_PASS,
	SLOT1_PAN,

	SLOT2_VOLUME,
	SLOT2_LOW_PASS,
	SLOT2_HIGH_PASS,
	SLOT2_PAN,

	SLOT3_VOLUME,
	SLOT3_LOW_PASS,
	SLOT3_HIGH_PASS,
	SLOT3_PAN,

	SLOT4_VOLUME,
	SLOT4_LOW_PASS,
	SLOT4_HIGH_PASS,
	SLOT4_PAN,

	SLOT5_VOLUME,
	SLOT5_LOW_PASS,
	SLOT5_HIGH_PASS,
	SLOT5_PAN,

	SLOT6_VOLUME,
	SLOT6_LOW_PASS,
	SLOT6_HIGH_PASS,
	SLOT6_PAN,

	SLOT7_VOLUME,
	SLOT7_LOW_PASS,
	SLOT7_HIGH_PASS,
	SLOT7_PAN,

	SLOT8_VOLUME,
	SLOT8_LOW_PASS,
	SLOT8_HIGH_PASS,
	SLOT8_PAN,

	SLOT9_VOLUME,
	SLOT9_LOW_PASS,
	SLOT9_HIGH_PASS,
	SLOT9_PAN,

	_END

};

/** one channel within the mixer */
struct SimpleMixerChannel {

	/** the volumne to use for this channel */
	Volume gain;

	/** low-pass filter for this channel */
	BiquadFilter bqLP;

	/** high-pass filter for this channel */
	BiquadFilter bqHP;

	/** paning betwen left and right */
	Volume pan;

	/** ctor */
	SimpleMixerChannel() : gain(1.0f), pan(0.5f) {;}

};


/**
 * combine several inputs to one output
 */
class SimpleMixer : public SoundProcessor {

public:

	/** ctor */
	SimpleMixer() : SoundProcessor(2*SM_SLOTS, 2), outputGain(1.0f) {

		// initialize filters
		for (unsigned int i = 0; i < SM_SLOTS; ++i) {
			sources[i].bqHP.setParameter(BiquadFilterParams::MODE, ParamValue(0, (int) BiquadFilterMode::_END - 1, (int) BiquadFilterMode::HIGH_PASS));
			sources[i].bqLP.setParameter(BiquadFilterParams::MODE, ParamValue(0, (int) BiquadFilterMode::_END - 1, (int) BiquadFilterMode::LOW_PASS));
			sources[i].bqHP.setParameter(BiquadFilterParams::CUTOFF_SIZE, 0.2f);
			sources[i].bqLP.setParameter(BiquadFilterParams::CUTOFF_SIZE, 0.2f);
			sources[i].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 0.0f);
			sources[i].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f);
		}


	}

	/** add output of one channel */
	void addToOutput(unsigned int idx, Amplitude** inputs, Amplitude** outputs, SimpleMixerChannel& chan, bool left) {

		float gain = chan.gain;
		gain *= (float) ( (left) ? std::sin((1.0 - chan.pan) * K_PI / 2) : (std::sin(chan.pan*K_PI/2)) );

		Amplitude* input = inputs[idx];
		Amplitude* output = (left) ? (outputs[0]) : (outputs[1]);

		// channel used?
		if (!input) {return;}

		// process
		for (SampleFrame i = 0; i < getSamplesPerProcess(); ++i) {
			Amplitude a = input[i] * gain;
			output[i] += a;
			vuInp[idx/2].add(a);
		}

	}

	void filterOutput(Amplitude** inputs, Amplitude** outputs, unsigned int slot) {
		if (inputs[slot*2] == nullptr) {return;}
		sources[slot].bqHP.process(&inputs[slot*2], &outputs[slot*2]);
		sources[slot].bqLP.process(&inputs[slot*2], &outputs[slot*2]);
	}

	/** get the mixed output */
	void process(Amplitude** inputs, Amplitude** outputs) {

		vuInp[0].reset();
		filterOutput(inputs, inputs, 0);
		addToOutput(0, inputs, outputs, sources[0], true);
		addToOutput(1, inputs, outputs, sources[0], false);

		vuInp[1].reset();
		filterOutput(inputs, inputs, 1);
		addToOutput(2, inputs, outputs, sources[1], true);
		addToOutput(3, inputs, outputs, sources[1], false);

		vuInp[2].reset();
		filterOutput(inputs, inputs, 2);
		addToOutput(4, inputs, outputs, sources[2], true);
		addToOutput(5, inputs, outputs, sources[2], false);

		vuInp[3].reset();
		filterOutput(inputs, inputs, 3);
		addToOutput(6, inputs, outputs, sources[3], true);
		addToOutput(7, inputs, outputs, sources[3], false);

		vuInp[4].reset();
		filterOutput(inputs, inputs, 4);
		addToOutput(8, inputs, outputs, sources[4], true);
		addToOutput(9, inputs, outputs, sources[4], false);

		vuInp[5].reset();
		filterOutput(inputs, inputs, 5);
		addToOutput(10, inputs, outputs, sources[5], true);
		addToOutput(11, inputs, outputs, sources[5], false);

		vuInp[6].reset();
		filterOutput(inputs, inputs, 6);
		addToOutput(12, inputs, outputs, sources[6], true);
		addToOutput(13, inputs, outputs, sources[6], false);

		vuInp[7].reset();
		filterOutput(inputs, inputs, 7);
		addToOutput(14, inputs, outputs, sources[7], true);
		addToOutput(15, inputs, outputs, sources[7], false);

		vuInp[8].reset();
		filterOutput(inputs, inputs, 8);
		addToOutput(16, inputs, outputs, sources[8], true);
		addToOutput(17, inputs, outputs, sources[8], false);

		// apply output volume
		for (SampleFrame i = 0; i < getSamplesPerProcess(); ++i) {
			outputs[0][i] *= outputGain;
			outputs[1][i] *= outputGain;
		}

		// VU
		vu.get(outputs[0], getSamplesPerProcess());

	}


	void setParameter(Param p, ParamValue v) override {
		SimpleMixerParams pp = (SimpleMixerParams) p;
		switch (pp) {

		case SimpleMixerParams::SLOT1_VOLUME:		sources[0].gain = Units::valueToGain(v); break;
		case SimpleMixerParams::SLOT2_VOLUME:		sources[1].gain = Units::valueToGain(v); break;
		case SimpleMixerParams::SLOT3_VOLUME:		sources[2].gain = Units::valueToGain(v); break;
		case SimpleMixerParams::SLOT4_VOLUME:		sources[3].gain = Units::valueToGain(v); break;
		case SimpleMixerParams::SLOT5_VOLUME:		sources[4].gain = Units::valueToGain(v); break;
		case SimpleMixerParams::SLOT6_VOLUME:		sources[5].gain = Units::valueToGain(v); break;
		case SimpleMixerParams::SLOT7_VOLUME:		sources[6].gain = Units::valueToGain(v); break;
		case SimpleMixerParams::SLOT8_VOLUME:		sources[7].gain = Units::valueToGain(v); break;
		case SimpleMixerParams::SLOT9_VOLUME:		sources[8].gain = Units::valueToGain(v); break;

		case SimpleMixerParams::SLOT1_HIGH_PASS:	sources[0].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;
		case SimpleMixerParams::SLOT2_HIGH_PASS:	sources[1].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;
		case SimpleMixerParams::SLOT3_HIGH_PASS:	sources[2].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;
		case SimpleMixerParams::SLOT4_HIGH_PASS:	sources[3].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;
		case SimpleMixerParams::SLOT5_HIGH_PASS:	sources[4].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;
		case SimpleMixerParams::SLOT6_HIGH_PASS:	sources[5].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;
		case SimpleMixerParams::SLOT7_HIGH_PASS:	sources[6].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;
		case SimpleMixerParams::SLOT8_HIGH_PASS:	sources[7].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;
		case SimpleMixerParams::SLOT9_HIGH_PASS:	sources[8].bqHP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, v); break;

		case SimpleMixerParams::SLOT1_LOW_PASS:		sources[0].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;
		case SimpleMixerParams::SLOT2_LOW_PASS:		sources[1].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;
		case SimpleMixerParams::SLOT3_LOW_PASS:		sources[2].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;
		case SimpleMixerParams::SLOT4_LOW_PASS:		sources[3].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;
		case SimpleMixerParams::SLOT5_LOW_PASS:		sources[4].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;
		case SimpleMixerParams::SLOT6_LOW_PASS:		sources[5].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;
		case SimpleMixerParams::SLOT7_LOW_PASS:		sources[6].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;
		case SimpleMixerParams::SLOT8_LOW_PASS:		sources[7].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;
		case SimpleMixerParams::SLOT9_LOW_PASS:		sources[8].bqLP.setParameter(BiquadFilterParams::CUTOFF_FREQUENCY, 1.0f - v); break;

		case SimpleMixerParams::SLOT1_PAN:			sources[0].pan = v; break;
		case SimpleMixerParams::SLOT2_PAN:			sources[1].pan = v; break;
		case SimpleMixerParams::SLOT3_PAN:			sources[2].pan = v; break;
		case SimpleMixerParams::SLOT4_PAN:			sources[3].pan = v; break;
		case SimpleMixerParams::SLOT5_PAN:			sources[4].pan = v; break;
		case SimpleMixerParams::SLOT6_PAN:			sources[5].pan = v; break;
		case SimpleMixerParams::SLOT7_PAN:			sources[6].pan = v; break;
		case SimpleMixerParams::SLOT8_PAN:			sources[7].pan = v; break;
		case SimpleMixerParams::SLOT9_PAN:			sources[8].pan = v; break;

		case SimpleMixerParams::MASTER_VOLUME:		outputGain = Units::valueToGain(v); break;

		default:				break;

		}
	}

	ParamValue getParameter(Param p) const override {
		SimpleMixerParams pp = (SimpleMixerParams) p;
		switch (pp) {
		case SimpleMixerParams::SLOT1_VOLUME:		return Units::gainToValue(sources[0].gain);
		case SimpleMixerParams::SLOT2_VOLUME:		return Units::gainToValue(sources[1].gain);
		case SimpleMixerParams::SLOT3_VOLUME:		return Units::gainToValue(sources[2].gain);
		case SimpleMixerParams::SLOT4_VOLUME:		return Units::gainToValue(sources[3].gain);
		case SimpleMixerParams::SLOT5_VOLUME:		return Units::gainToValue(sources[4].gain);
		case SimpleMixerParams::SLOT6_VOLUME:		return Units::gainToValue(sources[5].gain);
		case SimpleMixerParams::SLOT7_VOLUME:		return Units::gainToValue(sources[6].gain);
		case SimpleMixerParams::SLOT8_VOLUME:		return Units::gainToValue(sources[7].gain);
		case SimpleMixerParams::SLOT9_VOLUME:		return Units::gainToValue(sources[8].gain);

		case SimpleMixerParams::SLOT1_HIGH_PASS:	return sources[0].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT2_HIGH_PASS:	return sources[1].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT3_HIGH_PASS:	return sources[2].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT4_HIGH_PASS:	return sources[3].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT5_HIGH_PASS:	return sources[4].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT6_HIGH_PASS:	return sources[5].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT7_HIGH_PASS:	return sources[6].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT8_HIGH_PASS:	return sources[7].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT9_HIGH_PASS:	return sources[8].bqHP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);

		case SimpleMixerParams::SLOT1_LOW_PASS:		return 1.0f - sources[0].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT2_LOW_PASS:		return 1.0f - sources[1].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT3_LOW_PASS:		return 1.0f - sources[2].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT4_LOW_PASS:		return 1.0f - sources[3].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT5_LOW_PASS:		return 1.0f - sources[4].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT6_LOW_PASS:		return 1.0f - sources[5].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT7_LOW_PASS:		return 1.0f - sources[6].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT8_LOW_PASS:		return 1.0f - sources[7].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);
		case SimpleMixerParams::SLOT9_LOW_PASS:		return 1.0f - sources[8].bqLP.getParameter(BiquadFilterParams::CUTOFF_FREQUENCY);

		case SimpleMixerParams::SLOT1_PAN:			return sources[0].pan;
		case SimpleMixerParams::SLOT2_PAN:			return sources[1].pan;
		case SimpleMixerParams::SLOT3_PAN:			return sources[2].pan;
		case SimpleMixerParams::SLOT4_PAN:			return sources[3].pan;
		case SimpleMixerParams::SLOT5_PAN:			return sources[4].pan;
		case SimpleMixerParams::SLOT6_PAN:			return sources[5].pan;
		case SimpleMixerParams::SLOT7_PAN:			return sources[6].pan;
		case SimpleMixerParams::SLOT8_PAN:			return sources[7].pan;
		case SimpleMixerParams::SLOT9_PAN:			return sources[8].pan;

		case SimpleMixerParams::MASTER_VOLUME:		return Units::gainToValue(outputGain);

		default:									return ParamValue(0.0f);

		}
	}


	const std::string getParameterName(Param p) const override {
		SimpleMixerParams pp = (SimpleMixerParams) p;
		switch (pp) {

		case SimpleMixerParams::SLOT1_VOLUME:		return "SLOT1_VOLUME";
		case SimpleMixerParams::SLOT2_VOLUME:		return "SLOT2_VOLUME";
		case SimpleMixerParams::SLOT3_VOLUME:		return "SLOT3_VOLUME";
		case SimpleMixerParams::SLOT4_VOLUME:		return "SLOT4_VOLUME";
		case SimpleMixerParams::SLOT5_VOLUME:		return "SLOT5_VOLUME";
		case SimpleMixerParams::SLOT6_VOLUME:		return "SLOT6_VOLUME";
		case SimpleMixerParams::SLOT7_VOLUME:		return "SLOT7_VOLUME";
		case SimpleMixerParams::SLOT8_VOLUME:		return "SLOT8_VOLUME";
		case SimpleMixerParams::SLOT9_VOLUME:		return "SLOT9_VOLUME";

		case SimpleMixerParams::SLOT1_HIGH_PASS:	return "SLOT1_HIGH_PASS";
		case SimpleMixerParams::SLOT2_HIGH_PASS:	return "SLOT2_HIGH_PASS";
		case SimpleMixerParams::SLOT3_HIGH_PASS:	return "SLOT3_HIGH_PASS";
		case SimpleMixerParams::SLOT4_HIGH_PASS:	return "SLOT4_HIGH_PASS";
		case SimpleMixerParams::SLOT5_HIGH_PASS:	return "SLOT5_HIGH_PASS";
		case SimpleMixerParams::SLOT6_HIGH_PASS:	return "SLOT6_HIGH_PASS";
		case SimpleMixerParams::SLOT7_HIGH_PASS:	return "SLOT7_HIGH_PASS";
		case SimpleMixerParams::SLOT8_HIGH_PASS:	return "SLOT8_HIGH_PASS";
		case SimpleMixerParams::SLOT9_HIGH_PASS:	return "SLOT9_HIGH_PASS";

		case SimpleMixerParams::SLOT1_LOW_PASS:	return "SLOT1_LOW_PASS";
		case SimpleMixerParams::SLOT2_LOW_PASS:	return "SLOT2_LOW_PASS";
		case SimpleMixerParams::SLOT3_LOW_PASS:	return "SLOT3_LOW_PASS";
		case SimpleMixerParams::SLOT4_LOW_PASS:	return "SLOT4_LOW_PASS";
		case SimpleMixerParams::SLOT5_LOW_PASS:	return "SLOT5_LOW_PASS";
		case SimpleMixerParams::SLOT6_LOW_PASS:	return "SLOT6_LOW_PASS";
		case SimpleMixerParams::SLOT7_LOW_PASS:	return "SLOT7_LOW_PASS";
		case SimpleMixerParams::SLOT8_LOW_PASS:	return "SLOT8_LOW_PASS";
		case SimpleMixerParams::SLOT9_LOW_PASS:	return "SLOT9_LOW_PASS";

		case SimpleMixerParams::SLOT1_PAN:		return "SLOT1_PAN";
		case SimpleMixerParams::SLOT2_PAN:		return "SLOT2_PAN";
		case SimpleMixerParams::SLOT3_PAN:		return "SLOT3_PAN";
		case SimpleMixerParams::SLOT4_PAN:		return "SLOT4_PAN";
		case SimpleMixerParams::SLOT5_PAN:		return "SLOT5_PAN";
		case SimpleMixerParams::SLOT6_PAN:		return "SLOT6_PAN";
		case SimpleMixerParams::SLOT7_PAN:		return "SLOT7_PAN";
		case SimpleMixerParams::SLOT8_PAN:		return "SLOT8_PAN";
		case SimpleMixerParams::SLOT9_PAN:		return "SLOT9_PAN";

		case SimpleMixerParams::MASTER_VOLUME:		return "MASTER_VOLUME";

		default:				return "UNKNOWN";

		}
	}
	unsigned int getNumParameters() const override {
		return (int) SimpleMixerParams::_END - 1;
	}

	std::string getProductString() const override {
		return "SimpleMixer";
	}

	bool getInputProperties(unsigned int idx, PinProperties* properties) override {
		switch(idx) {
		case 0:		properties->name = "input ch1 (left)"; break;
		case 1:		properties->name = "input ch1 (right)"; break;
		case 2:		properties->name = "input ch2 (left)"; break;
		case 3:		properties->name = "input ch2 (right)"; break;
		case 4:		properties->name = "input ch3 (left)"; break;
		case 5:		properties->name = "input ch3 (right)"; break;
		case 6:		properties->name = "input ch4 (left)"; break;
		case 7:		properties->name = "input ch4 (right)"; break;
		case 8:		properties->name = "input ch5 (left)"; break;
		case 9:		properties->name = "input ch5 (right)"; break;
		case 10:	properties->name = "input ch6 (left)"; break;
		case 11:	properties->name = "input ch6 (right)"; break;
		case 12:	properties->name = "input ch7 (left)"; break;
		case 13:	properties->name = "input ch7 (right)"; break;
		case 14:	properties->name = "input ch8 (left)"; break;
		case 15:	properties->name = "input ch8 (right)"; break;
		case 16:	properties->name = "input ch9 (left)"; break;
		case 17:	properties->name = "input ch9 (right)"; break;
		}
		return true;
	}

	bool getOutputProperties(unsigned int idx, PinProperties* properties) override {
		switch(idx) {
		case 0:
			properties->name = "output (left)";
			break;
		case 1:
			properties->name = "output (right)";
			break;
		}
		return true;
	}


	Volume getVU() override {
		return vu.getMax();
	}

	/** is the HP filter for slot x currently active? */
	bool isHPActive(unsigned int slot) {return sources[slot].bqHP.isActive();}

	/** is the LP filter for slot x currently active? */
	bool isLPActive(unsigned int slot) {return sources[slot].bqLP.isActive();}


protected:

	/** get VU for the given input channel */
	Amplitude getVU(unsigned int channel) {
		return vuInp[channel].getMax();
	}

private:

	/** all available slots */
	SimpleMixerChannel sources[SM_SLOTS];

	/** the overall output volume */
	Volume outputGain;

	/** vu calculator */
	VUCalc vu;

	/** vu calculator for each input slot */
	VUCalc vuInp[SM_SLOTS];


};



#endif /* SIMPLEMIXER_H_ */
