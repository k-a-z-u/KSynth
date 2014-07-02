#ifndef ALLPASSREVERB_H
#define ALLPASSREVERB_H

#include "../misc/SoundProcessor.h"
#include "AllPassReverbGate.h"

/** all configurable params for the AllPassReverb */
enum class AllPassReverbParams {

	GATE1A_DELAY,
	GATE1A_POS_GAIN,
	GATE1A_NEG_GAIN,

	GATE1B_DELAY,
	GATE1B_POS_GAIN,
	GATE1B_NEG_GAIN,

	GATE2A_DELAY,
	GATE2A_POS_GAIN,
	GATE2A_NEG_GAIN,

	GATE2B_DELAY,
	GATE2B_POS_GAIN,
	GATE2B_NEG_GAIN,

	DRY_WET,

	_END

};

// the maximum delay uses 10% of the delay buffers size
#define APR_MAX_DELAY	0.1f

/**
 * an AllPassReverb filter using several (comb-filter/all-pass) gates
 * http://www.earlevel.com/main/1997/01/19/a-bit-about-reverb/
 */
class AllPassReverb : public SoundProcessor {

public:

	/** ctor */
	AllPassReverb() : SoundProcessor(2,2), dryWet(0.25f) {
		dryWet = 0.5f;
	}


	void setParameter(AllPassReverbParams p, ParamValue v) {

		switch (p) {

			case AllPassReverbParams::GATE1A_DELAY:			gates[0].setDelay(v * APR_MAX_DELAY); break;
			case AllPassReverbParams::GATE1A_POS_GAIN:		gates[0].setPositiveGain(v); break;
			case AllPassReverbParams::GATE1A_NEG_GAIN:		gates[0].setNegativeGain(v); break;

			case AllPassReverbParams::GATE1B_DELAY:			gates[1].setDelay(v * APR_MAX_DELAY); break;
			case AllPassReverbParams::GATE1B_POS_GAIN:		gates[1].setPositiveGain(v); break;
			case AllPassReverbParams::GATE1B_NEG_GAIN:		gates[1].setNegativeGain(v); break;

			case AllPassReverbParams::GATE2A_DELAY:			gates[2].setDelay(v * APR_MAX_DELAY); break;
			case AllPassReverbParams::GATE2A_POS_GAIN:		gates[2].setPositiveGain(v); break;
			case AllPassReverbParams::GATE2A_NEG_GAIN:		gates[2].setNegativeGain(v); break;

			case AllPassReverbParams::GATE2B_DELAY:			gates[3].setDelay(v * APR_MAX_DELAY); break;
			case AllPassReverbParams::GATE2B_POS_GAIN:		gates[3].setPositiveGain(v); break;
			case AllPassReverbParams::GATE2B_NEG_GAIN:		gates[3].setNegativeGain(v); break;

			case AllPassReverbParams::DRY_WET:				dryWet = v; break;

			default:										break;

		}

	}

	ParamValue getParameter(AllPassReverbParams p) const {

		switch (p) {

			case AllPassReverbParams::GATE1A_DELAY:			return gates[0].getDelay() / APR_MAX_DELAY;
			case AllPassReverbParams::GATE1A_POS_GAIN:		return gates[0].getPositiveGain();
			case AllPassReverbParams::GATE1A_NEG_GAIN:		return gates[0].getNegativeGain();

			case AllPassReverbParams::GATE1B_DELAY:			return gates[1].getDelay() / APR_MAX_DELAY;
			case AllPassReverbParams::GATE1B_POS_GAIN:		return gates[1].getPositiveGain();
			case AllPassReverbParams::GATE1B_NEG_GAIN:		return gates[1].getNegativeGain();

			case AllPassReverbParams::GATE2A_DELAY:			return gates[2].getDelay() / APR_MAX_DELAY;
			case AllPassReverbParams::GATE2A_POS_GAIN:		return gates[2].getPositiveGain();
			case AllPassReverbParams::GATE2A_NEG_GAIN:		return gates[2].getNegativeGain();

			case AllPassReverbParams::GATE2B_DELAY:			return gates[3].getDelay() / APR_MAX_DELAY;
			case AllPassReverbParams::GATE2B_POS_GAIN:		return gates[3].getPositiveGain();
			case AllPassReverbParams::GATE2B_NEG_GAIN:		return gates[3].getNegativeGain();

			case AllPassReverbParams::DRY_WET:				return dryWet;

			default:										return -1.0f;

		}

	}

	void setParameter(Param p, ParamValue v) override {
		setParameter( (AllPassReverbParams) p, v );
	}

	ParamValue getParameter(Param p) const override {
		return getParameter( (AllPassReverbParams) p );
	}


	bool getInputProperties(const unsigned int idx, PinProperties* properties) const override {
		switch(idx) {
		case 0:		properties->name = "input ch1 (left)"; break;
		case 1:		properties->name = "input ch1 (right)"; break;
		}
		return true;
	}

	bool getOutputProperties(const unsigned int idx, PinProperties* properties) const override {
		switch(idx) {
		case 0:		properties->name = "output (left)"; break;
		case 1:		properties->name = "output (right)"; break;
		}
		return true;
	}


	void process(Amplitude** input, Amplitude** output) override {

		float negDW = 1.0f - dryWet;

		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {

			AudioData in(input[0][i], input[1][i]);
			AudioData reverbed = in;

			// perform reverb with 4 gates
			reverbed = gates[0].process(reverbed) + gates[1].process(reverbed);
			reverbed = gates[2].process(reverbed) + gates[3].process(reverbed);

			// combine output signal
			AudioData out = (reverbed * dryWet) + (in * negDW);

			// done
			output[0][i] = out.left;
			output[1][i] = out.right;

		}

	}

	unsigned int getNumParameters() const override {
		return (unsigned int) AllPassReverbParams::_END;
	}

	std::string getProductString() const override {
		return "AllPassReverb";
	}

protected:

	/** get an impulse response for the current configuration */
	void getImpulseResponse(const unsigned int size, float* target) {

		for (unsigned int i = 0; i < 4; ++i) {
			_gates[i].setDelay(gates[i].getDelay());
			_gates[i].setPositiveGain(gates[i].getPositiveGain());
			_gates[i].setNegativeGain(gates[i].getNegativeGain());
			_gates[i].reset();
		}

		for (unsigned int i = 0; i < size; ++i) {
			float out = (i == 0) ? (1) : (0);
			out = _gates[0].process(out) + _gates[1].process(out);
			out = _gates[2].process(out) + _gates[3].process(out);
			target[i] = out;
		}

	}

private:

	/** all available gates */
	AllPassReverbGate<AudioData, 65536> gates[4];

	/** gates for impulse response */
	AllPassReverbGate<float, 65536> _gates[4];

	/** dry/wet setting */
	float dryWet;

};

#endif // ALLPASSREVERB_H
