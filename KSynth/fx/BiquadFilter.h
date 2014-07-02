/*
 * BiquadFilter.h
 *
 *  Created on: Nov 4, 2013
 *      Author: kazu
 */

#ifndef BIQUADFILTER_H_
#define BIQUADFILTER_H_

#include "BiquadFilterGate.h"
#include "../misc/SoundProcessor.h"
#include "../misc/DataTypes.h"
#define BQ_MAX_KHZ		22050
#define BQ_MIN_HZ		30

#include "../misc/Units.h"

/** enum for each configurable parameter of the BiquadFilter */
enum class BiquadFilterParams {

	BYPASS,
	CUTOFF_FREQUENCY,
	CUTOFF_SIZE,
	GAIN,
	MODE,

	_END,

};

/** enum for each supported mode of the filter */
enum class BiquadFilterMode {

	LOW_PASS,
	HIGH_PASS,
	ALL_PASS,
	BAND_PASS,
	NOTCH,
	LOW_SHELF,
	HIGH_SHELF,

	_END

};

/**
 * a simple biquad filter that can be used
 * for low- or high-pass filtering
 *
 * http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 */
class BiquadFilter : public SoundProcessor, public BiquadFilterGate<2> {

public:

	/** ctor */
	BiquadFilter() : SoundProcessor(2, 2), bypass(false) {
		reset();
	}

	/** dtor */
	virtual ~BiquadFilter() {
		;
	}

	void process(Amplitude** inputs, Amplitude** outputs) override {

		if (bypass || disabled) {
			outputs[0] = inputs[0];
			outputs[1] = inputs[1];
			return;
		}

		Amplitude* iLeft = inputs[0];
		Amplitude* iRight = inputs[1];
		Amplitude* oLeft = outputs[0];
		Amplitude* oRight = outputs[1];

		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
			oLeft[i] = filter(0, iLeft[i]);
			oRight[i] = filter(1, iRight[i]);
		}

	}

	std::string getProductString() const override {
		return "BiQuad Filter";
	}

//	unsigned int paramToFreq(ParamValue v) const {
//		if (v == 0.0f) {return 0;}
//		if (v == 1.0f) {return BQ_MAX_KHZ;}
//		return (int) (-log10(1.0 - v) * BQ_MAX_KHZ / 2.0);
//	}

//	ParamValue freqToParam(unsigned int freq) const {
//		if (freq >= BQ_MAX_KHZ) {return 1.0f;}
//		if (freq == 0) {return 0.0f;}
//		float val = (float) (1.0 - pow(10, (-(int)freq * 2.0 / BQ_MAX_KHZ)));
//		return ParamValue( val );
//	}


	/** convenience method to provide an enum as parameter */
	void setParameter(BiquadFilterParams p, ParamValue v) {
		switch (p) {
			case BiquadFilterParams::BYPASS:			this->bypass = v.asBool(); break;
			case BiquadFilterParams::CUTOFF_FREQUENCY:	params.freqFact = Units::paramToLogFreqFact(v); reconfigure(); break;
			//case BiquadFilterParams::CUTOFF_FREQUENCY:params.freq = Units::floatToFreq(v, BQ_MAX_KHZ); reconfigure(); break;
			case BiquadFilterParams::CUTOFF_SIZE:		params.size = v.asFloat(0.0f, 8.0f); reconfigure(); break;
			case BiquadFilterParams::MODE:				params.mode = (BiquadFilterMode) v.asInt(0, (int)BiquadFilterMode::_END - 1); reconfigure(); break;
			case BiquadFilterParams::GAIN:				params.gain = v.asFloat(1.0f, 4.0f); reconfigure(); break;
			default:									return;
		}
	}

	void setParameter(Param p, ParamValue v) override {
		setParameter( (BiquadFilterParams) p, v );
	}

	/** convenience method to provide an enum as parameter */
	ParamValue getParameter(BiquadFilterParams p) const {
		switch (p) {
			case BiquadFilterParams::BYPASS:			return ParamValue(this->bypass);
			case BiquadFilterParams::CUTOFF_FREQUENCY:	return Units::logFreqFactToParam(params.freqFact);
			//case BiquadFilterParams::CUTOFF_FREQUENCY:return Units::freqToFloat(params.freq, BQ_MAX_KHZ);
			case BiquadFilterParams::CUTOFF_SIZE:		return ParamValue(0.0f, 8.0f, params.size);
			case BiquadFilterParams::GAIN:				return ParamValue(1.0f, 4.0f, params.gain);
			case BiquadFilterParams::MODE:				return ParamValue(0, (int) BiquadFilterMode::_END - 1, (int) params.mode);
			default:									return ParamValue(0.0f);
		}
	}

	ParamValue getParameter(Param p) const override {
		return getParameter( (BiquadFilterParams) p );
	}

	unsigned int getNumParameters() const override {
		return (int) BiquadFilterParams::_END;
	}


	/** is the biquad filter running? (not bypassed + correct params) */
	bool isActive() {
		return !bypass && !disabled;
	}

	/** apply all values of the param-struct */
	void reconfigure() {
		//reset();
		switch (params.mode) {
		case BiquadFilterMode::LOW_PASS:	setLowPass	(params.freqFact, params.size); break;
		case BiquadFilterMode::HIGH_PASS:	setHighPass	(params.freqFact, params.size); break;
		case BiquadFilterMode::ALL_PASS:	setAllPass	(params.freqFact, params.size); break;
		case BiquadFilterMode::BAND_PASS:	setBandPass	(params.freqFact, params.size); break;
		case BiquadFilterMode::NOTCH:		setNotch	(params.freqFact, params.size); break;
		case BiquadFilterMode::LOW_SHELF:	setLowShelf	(params.freqFact, params.size, params.gain); break;
		case BiquadFilterMode::HIGH_SHELF:	setHighShelf(params.freqFact, params.size, params.gain); break;
		case BiquadFilterMode::_END:		return;
		}
	}


private:

	struct Parameters {

		/** the requested filtering mode */
		BiquadFilterMode mode;

		/** the cut-off frequency between ]0;0.5[ */
		float freqFact;

		/** the cut-off size (in octatves) */
		float size;

		/** an additional gain parameter (if applicable to the filter-mode) */
		float gain;

		/** ctor */
		Parameters() : mode(BiquadFilterMode::_END), freqFact(0), size(0), gain(0) {;}

	} params;

	/** bypass this filter? */
	bool bypass;

};


#endif /* BIQUADFILTER_H_ */
