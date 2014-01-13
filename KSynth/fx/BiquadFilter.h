/*
 * BiquadFilter.h
 *
 *  Created on: Nov 4, 2013
 *      Author: kazu
 */

#ifndef BIQUADFILTER_H_
#define BIQUADFILTER_H_

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
class BiquadFilter : public SoundProcessor {

public:

	/** ctor */
	BiquadFilter() : SoundProcessor(2, 2), in(), out(), disabled(false), bypass(false) {
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
		case BiquadFilterParams::CUTOFF_FREQUENCY:	params.freq = Units::floatToFreq(v, BQ_MAX_KHZ); reconfigure(); break;
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
		case BiquadFilterParams::CUTOFF_FREQUENCY:	return Units::freqToFloat(params.freq, BQ_MAX_KHZ);
		case BiquadFilterParams::CUTOFF_SIZE:		return ParamValue(0.0f, 8.0f, params.size);
		case BiquadFilterParams::GAIN:				return ParamValue(1.0f, 4.0f, params.gain);
		case BiquadFilterParams::MODE:				return ParamValue(0, (int) BiquadFilterMode::_END - 1, (int) params.mode);
		default:									return ParamValue(0.0f);
		}
	}

	ParamValue getParameter(Param p) const override {
		return getParameter( (BiquadFilterParams) p );
	}

	/** is the biquad filter running? (not bypassed + correct params) */
	bool isActive() {
		return !bypass && !disabled;
	}


	/** apply all values of the param-struct */
	void reconfigure() {
		//reset();
		switch (params.mode) {
		case BiquadFilterMode::LOW_PASS:	setLowPass(params.freq, params.size); break;
		case BiquadFilterMode::HIGH_PASS:	setHighPass(params.freq, params.size); break;
		case BiquadFilterMode::ALL_PASS:	setAllPass(params.freq, params.size); break;
		case BiquadFilterMode::BAND_PASS:	setBandPass(params.freq, params.size); break;
		case BiquadFilterMode::NOTCH:		setNotch(params.freq, params.size); break;
		case BiquadFilterMode::LOW_SHELF:	setLowShelf(params.freq, params.size, params.gain); break;
		case BiquadFilterMode::HIGH_SHELF:	setHighShelf(params.freq, params.size, params.gain); break;
		case BiquadFilterMode::_END:		return;
		}
	}

	/** filter the given amplitude of the given channel (history) */
	Amplitude filter(unsigned int ch, Amplitude aIn) {

		Amplitude aOut = 0;
		aOut += aIn       *(b0a0);
		aOut += in[ch][0] *(b1a0);
		aOut += in[ch][1] *(b2a0);
		aOut -= out[ch][0]*(a1a0);
		aOut -= out[ch][1]*(a2a0);

		//aOut = clamp1(aOut);

		in[ch][1] = in[ch][0];
		in[ch][0] = aIn;

		out[ch][1] = out[ch][0];
		out[ch][0] = aOut;

		return aOut;

	}


	/** reset (disable) the filter */
	void reset() {
		b0a0 = 1.0;
		b1a0 = 0.0;
		b2a0 = 0.0;
		a1a0 = 0.0;
		a2a0 = 0.0;
		in[0][0] = 0;
		in[0][1] = 0;
		in[1][0] = 0;
		in[1][1] = 0;
		out[0][0] = 0;
		out[0][1] = 0;
		out[1][0] = 0;
		out[1][1] = 0;
	}



	/** configure the filter as low-pass */
	void setLowPass(Frequency freq, float octaves) {

		if (freq <= BQ_MIN_HZ)	{freq = BQ_MIN_HZ;}
		if (freq >= BQ_MAX_KHZ)	{disabled = true; return;}
		disabled = false;

		double w0 = 2*K_PI*freq/getSampleRate();
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =  (1.0 - cos(w0))/2.0;
		double b1 =   1.0 - cos(w0);
		double b2 =  (1.0 - cos(w0))/2.0;
		double a0 =   1.0 + alpha;
		double a1 =  -2.0*cos(w0);
		double a2 =   1.0 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as low-pass */

	/*
	 *		double g = pow(10.0, 0.05 * res);
		double d = sqrt((4 - sqrt(16 - 16 / (g * g))) / 2);

		double theta = K_PI2 * freq;
		double sn = 0.5 * d * sin(theta);
		double beta = 0.5 * (1 - sn) / (1 + sn);
		double gamma = (0.5 + beta) * cos(theta);
		double alpha = 0.25 * (0.5 + beta - gamma);

		double a0 = 1;
		double b0 = 2 * alpha;
		double b1 = 2 * 2 * alpha;
		double b2 = 2 * alpha;
		double a1 = 2 * -gamma;
		double a2 = 2 * beta;

		b0a0 = b0/a0;
		b1a0 = b1/a0;
		b2a0 = b2/a0;
		a2a0 = a2/a0;
		a1a0 = a1/a0;
		*/

	//http://dspwiki.com/index.php?title=Lowpass_Resonant_Biquad_Filter

	//http://www.opensource.apple.com/source/WebCore/WebCore-7536.26.14/platform/audio/Biquad.cpp
	void setLowPassResonance(float freq, float res) {

		if (freq <= 0) {freq = 0.001f;}
		if (freq >= 1) {disabled = true; return;}

//		if (freq <= 0) {freq = 10;}
//		if (freq >= BQ_MAX_KHZ) {disabled = true; return;}
		disabled = false;

//		float cfreq = pow((float)1000.0,(float)freq)*20;
//		if(cfreq < 19) {cfreq = 19;}
//		//res=res*-1+1;

//		double c  =  1.0 / tan(K_PI*cfreq/getSampleRate());

//		b0a0 = 1.0 / (1.0 + res * c + c * c);
//		b1a0 = 2.0 * b0a0;
//		b2a0 = b0a0;
//		a2a0 = 2.0 * (1.0 - c * c) * b0a0;
//		a1a0 = ( 1.0 - res * c + c * c) * b0a0;

		res *= 10;

		double g = pow(10.0, 0.05 * res);
		double d = sqrt((4 - sqrt(16 - 16 / (g * g))) / 2);

		double theta = K_PI * freq;
		double sn = 0.5 * d * sin(theta);
		double beta = 0.5 * (1 - sn) / (1 + sn);
		double gamma = (0.5 + beta) * cos(theta);
		double alpha = 0.25 * (0.5 + beta - gamma);

		double a0 = 1.0;
		double b0 = 2.0 * alpha;
		double b1 = 2.0 * 2.0 * alpha;
		double b2 = 2.0 * alpha;
		double a1 = 2.0 * -gamma;
		double a2 = 2.0 * beta;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as high-pass */

	void setHighPass(Frequency freq, float octaves) {

		if (freq <= BQ_MIN_HZ)	{disabled = true; return;}
		if (freq >= BQ_MAX_KHZ)	{freq = BQ_MAX_KHZ;}
		disabled = false;

		double w0 = 2*K_PI*freq/getSampleRate();
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =  (1.0 + cos(w0))/2.0;
		double b1 = -(1.0 + cos(w0));
		double b2 =  (1.0 + cos(w0))/2.0;
		double a0 =   1.0 + alpha;
		double a1 =  -2.0*cos(w0);
		double a2 =   1.0 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as band-pass */
	void setBandPass(Frequency freq, float octaves) {

		if (freq <= 0) {disabled = true; return;}
		disabled = false;

		double w0 = 2*K_PI*freq/2/getSampleRate();
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =   sin(w0)/2.0;
		double b1 =   0.0;
		double b2 =  -sin(w0)/2.0;
		double a0 =   1.0 + alpha;
		double a1 =  -2.0*cos(w0);
		double a2 =   1.0 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as all-pass */
	void setAllPass(Frequency freq, float octaves) {

		double w0 = 2*K_PI*freq/getSampleRate();
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =   1 - alpha;
		double b1 =  -2*cos(w0);
		double b2 =   1 + alpha;
		double a0 =   1 + alpha;
		double a1 =  -2*cos(w0);
		double a2 =   1 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	void setNotch(Frequency freq, float octaves) {

		double w0 = 2*K_PI*freq/getSampleRate();
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =   1.0;
		double b1 =  -2.0*cos(w0);
		double b2 =   1.0;
		double a0 =   1.0 + alpha;
		double a1 =  -2.0*cos(w0);
		double a2 =   1.0 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as low-shelf. increase all aplitudes below freq? */
	void setLowShelf(Frequency freq, float octaves, float gain) {

		double A = sqrt( pow(10, (gain/20.0)) );
		double w0 = 2*K_PI*freq/getSampleRate();
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =     A*( (A+1.0) - (A-1.0)*cos(w0) + 2.0*sqrt(A)*alpha );
		double b1 = 2.0*A*( (A-1.0) - (A+1.0)*cos(w0)                   );
		double b2 =     A*( (A+1.0) - (A-1.0)*cos(w0) - 2.0*sqrt(A)*alpha );
		double a0 =         (A+1.0) + (A-1.0)*cos(w0) + 2.0*sqrt(A)*alpha;
		double a1 =  -2.0*( (A-1.0) + (A+1.0)*cos(w0)                   );
		double a2 =         (A+1.0) + (A-1.0)*cos(w0) - 2.0*sqrt(A)*alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as high-shelf. increase all amplitues above freq? */
	void setHighShelf(Frequency freq, float octaves, float gain) {

		double A = sqrt( pow(10, (gain/20.0)) );
		double w0 = 2*K_PI*freq/getSampleRate();
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =      A*( (A+1.0) + (A-1.0)*cos(w0) + 2.0*sqrt(A)*alpha );
		double b1 = -2.0*A*( (A-1.0) + (A+1.0)*cos(w0)                   );
		double b2 =      A*( (A+1.0) + (A-1.0)*cos(w0) - 2.0*sqrt(A)*alpha );
		double a0 =          (A+1.0) - (A-1.0)*cos(w0) + 2.0*sqrt(A)*alpha;
		double a1 =    2.0*( (A-1.0) - (A+1.0)*cos(w0)                   );
		double a2 =          (A+1.0) - (A-1.0)*cos(w0) - 2.0*sqrt(A)*alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	unsigned int getNumParameters() const override {
		return (int) BiquadFilterParams::_END - 1;
	}


private:

	/** calculate quotients */
	void setValues(double a0, double a1, double a2, double b0, double b1, double b2) {
		b0a0 = float(b0/a0);
		b1a0 = float(b1/a0);
		b2a0 = float(b2/a0);
		a2a0 = float(a2/a0);
		a1a0 = float(a1/a0);
	}

	struct Parameters {

		/** the requested filtering mode */
		BiquadFilterMode mode;

		/** the cut-off frequency */
		Frequency freq;

		/** the cut-off size (in octatves) */
		float size;

		/** an additional gain parameter (if applicable to the filter-mode) */
		float gain;

		/** ctor */
		Parameters() : mode(BiquadFilterMode::_END), freq(0), size(0), gain(0) {;}

	} params;


	float b0a0;
	float b1a0;
	float b2a0;

	float a1a0;
	float a2a0;

	Amplitude in[2][2];
	Amplitude out[2][2];

	/** filter disabled due to wrong params? */
	bool disabled;

	/** bypass this filter? */
	bool bypass;

};


#endif /* BIQUADFILTER_H_ */
