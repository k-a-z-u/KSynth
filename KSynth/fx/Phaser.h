#ifndef PHASER_H
#define PHASER_H

#include "BiquadFilterGate.h"
#include "../misc/SoundProcessor.h"
#include "../oscillator/SimpleOscillator2.h"

/** max SampleRate * 0.49 */
#define PHASER_FILTER_MAX		0.48f
/** min SampleRate * 0.01 */
#define PHASER_FILTER_MIN		0.02f
/** the region between both values */
#define PHASER_FILTER_REGION	(PHASER_FILTER_MAX - PHASER_FILTER_MIN)
/** the maximum size around the current frequency */
#define FILTER_FILTER_MAX_SIZE	5.0f;


/** all configurable params for the AllPassReverb */
enum class PhaserParams {

	/** dry/wet ratio */
	DRY_WET,

	/** the lower-end for the allpass */
	FILTER_MIN,

	/** the heigher-end for the allpass */
	FILTER_MAX,

	/** the "size" to use for the allpass around the current frequency */
	FILTER_SIZE,

	/** the oscillation function to use (sin, square...) */
	OSC_FUNCTION,

	/** the oscillation frequency */
	OSC_FREQ,

	_END

};

/**
 * a very simple phaser effect
 */
class Phaser : public SoundProcessor {

public:

	/** ctor */
	Phaser() : SoundProcessor(2,2), _updateCnt(0) {

		allPass.setAllPass(0.25, 0.5);
		lfo.setMode(SimpleOscillator2Mode::SINE);

		filter.min = PHASER_FILTER_MIN;
		filter.max = PHASER_FILTER_MAX * 0.8f;
		filter.curPhase = 0.0;
		filter.dryWet = 0.5f;
		filter.freq = 0.5f;
		filter.size = 2.0f;

	}

	void process(Amplitude** inputs, Amplitude** outputs) override {

		// pre-calculate the mixing
		Volume mix1 = filter.dryWet;
		Volume mix2 = 1.0f - filter.dryWet;

		// calculate the phase increment
		double phaseInc = filter.freq / (double) getSampleRate();

		// filter
		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {

			// only update the filter every 32 samples (performance reasons
			if (++_updateCnt >= 32) {
				_updateCnt = 0;

				// adjust the phase
				filter.curPhase += phaseInc * 32;
				float phase01 ((lfo.get((float)filter.curPhase) + 1.0f) * 0.5f);		// alternates between [0,1]
				float center = filter.min + phase01 * (filter.max - filter.min);

				// adjust the filter
				allPass.setAllPass(center, filter.size);

			}

			float left = inputs[0][i];
			left = allPass.filter(0, left);
			left = allPass.filter(1, left);
			left = allPass.filter(2, left);
			left = allPass.filter(3, left);

			float right = inputs[1][i];
			right = allPass.filter(4, right);
			right = allPass.filter(5, right);
			right = allPass.filter(6, right);
			right = allPass.filter(7, right);

			// calculate output sample
			outputs[0][i] = inputs[0][i] * mix1 + left * mix2;
			outputs[1][i] = inputs[1][i] * mix1 + right * mix2;

		}


	}

	std::string getProductString() const override {
		return "Phaser";
	}

	void setParameter(PhaserParams p, ParamValue v) {

		switch (p) {

			case PhaserParams::DRY_WET:						filter.dryWet = v; break;
			case PhaserParams::FILTER_MAX:
				filter.max = paramToMinMax(v);
				if (filter.max < filter.min) {filter.min = filter.max;}
				break;

			case PhaserParams::FILTER_MIN:
				filter.min = paramToMinMax(v);
				if (filter.min > filter.max) {filter.max = filter.min;}
				break;

			case PhaserParams::FILTER_SIZE:					filter.size = v * FILTER_FILTER_MAX_SIZE; break;

			case PhaserParams::OSC_FREQ:					filter.freq = paramToFreq(v); break;
			case PhaserParams::OSC_FUNCTION:				lfo.setMode( (SimpleOscillator2Mode) v.asInt(0, (int)SimpleOscillator2Mode::_END-1) ); break;

			default:										break;

		}

	}

	ParamValue getParameter(PhaserParams p) const {

		switch (p) {

			case PhaserParams::DRY_WET:						return filter.dryWet;
			case PhaserParams::FILTER_MAX:					return minMaxToParam(filter.max);
			case PhaserParams::FILTER_MIN:					return minMaxToParam(filter.min);
			case PhaserParams::FILTER_SIZE:					return filter.size / FILTER_FILTER_MAX_SIZE;
			case PhaserParams::OSC_FREQ:					return freqToParam(filter.freq);
			case PhaserParams::OSC_FUNCTION:				return ParamValue(0, (int)SimpleOscillator2Mode::_END-1, (int) lfo.getMode());

			default:										return -1.0f;

		}

	}

	void setParameter(Param p, ParamValue v) override {
		setParameter( (PhaserParams) p, v );
	}

	ParamValue getParameter(Param p) const override {
		return getParameter( (PhaserParams) p );
	}

	unsigned int getNumParameters() const override {
		return (unsigned int) PhaserParams::_END;
	}


private:

	/** convert [0,1] to [min,max] */
	Volume paramToMinMax(const ParamValue pv) const { return PHASER_FILTER_MIN + PHASER_FILTER_REGION * pv; }

	/** convert [min,max] to [0,1] */
	ParamValue minMaxToParam(const Volume minMax) const { return (minMax - PHASER_FILTER_MIN) / PHASER_FILTER_REGION; }

	Frequency paramToFreq(const ParamValue pv) const { return pv * 10.0f; }
	ParamValue freqToParam(const Frequency freq) const { return freq / 10.0f; }


private:

	/** the underlying all-pass filter */
	BiquadFilterGate<8> allPass;

	/** lfo to alter the settings */
	SimpleOscillator2 lfo;

	/** filter configuration */
	struct {
		Volume dryWet;
		Volume min;
		Volume max;
		Volume size;
		Frequency freq;
		double curPhase;
	} filter;

	int _updateCnt;

};

#endif // PHASER_H
