#ifndef BIQUADFILTERGATE_H
#define BIQUADFILTERGATE_H


/** lower frequency limit. about 20 Hz at 48.000 Hz */
#define BFG_MIN		0.0005
#define BFG_MAX		0.4995

#include "../misc/DataTypes.h"

/**
 * a simple biquad filter that can be used
 * for low- or high-pass filtering
 *
 * http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 */
template <int channels> class BiquadFilterGate {

public:

	/** ctor */
	BiquadFilterGate() : in(), out(), disabled(false) {
		reset();
	}

	/** filter the given amplitude of the given channel (history) */
	Amplitude filter( const unsigned int ch, const Amplitude aIn ) {

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

		memset(in, 0, sizeof(in));
		memset(out, 0, sizeof(out));

	}

	/** configure the filter as low-pass. freqFact between ]0;0.5[ */
	void setLowPass( double freqFact, const float octaves ) {

		if (freqFact < BFG_MIN)	{freqFact = BFG_MIN;}
		if (freqFact > BFG_MAX)	{disabled = true; return;}
		disabled = false;

		double w0 = 2.0 * K_PI * freqFact;
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
	void setLowPass( const Frequency freq, const float octaves, const SampleRate sRate ) {
		double freqFact = double(freq) / double(sRate);
		setLowPass(freqFact, octaves);
	}



	//http://dspwiki.com/index.php?title=Lowpass_Resonant_Biquad_Filter
	//http://www.opensource.apple.com/source/WebCore/WebCore-7536.26.14/platform/audio/Biquad.cpp
	/**
	 * configure as low-pass filter with resonance
	 * @param freqFact the frequency factor between ]0;0.5[
	 * @param res
	 */
	void setLowPassResonance( double freqFact, float res ) {

		if (freqFact < BFG_MIN)	{freqFact = BFG_MIN;}
		if (freqFact > BFG_MAX)	{disabled = true; return;}
		disabled = false;

		res *= 10;

		double g = pow(10.0, 0.05 * res);
		double d = sqrt((4 - sqrt(16 - 16 / (g * g))) / 2);

		double theta = K_PI * freqFact;
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

	/** configure the filter as high-pass. freqFact between ]0;0.5[ */
	void setHighPass( double freqFact, const float octaves ) {

		if (freqFact < BFG_MIN)	{disabled = true; return;}
		if (freqFact > BFG_MAX)	{freqFact = BFG_MAX;}
		disabled = false;

		double w0 = 2.0 * K_PI * freqFact;
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =  (1.0 + cos(w0))/2.0;
		double b1 = -(1.0 + cos(w0));
		double b2 =  (1.0 + cos(w0))/2.0;
		double a0 =   1.0 + alpha;
		double a1 =  -2.0*cos(w0);
		double a2 =   1.0 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as high-pass */
	void setHighPass( const Frequency freq, const float octaves, const SampleRate sRate ) {
		double freqFact = double(freq) / double(sRate);
		setHighPass(freqFact, octaves);
	}

	/** configure the filter as band-pass. freqFact between ]0;0.5[ */
	void setBandPass( double freqFact, const float octaves ) {

		if (freqFact < BFG_MIN) {disabled = true; return;}
		if (freqFact > BFG_MAX) {disabled = true; return;}
		disabled = false;

		//double w0 = 2 * K_PI * / 2 / freqFact;
		double w0 = 2.0 * K_PI * freqFact;
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =   sin(w0)/2.0;
		double b1 =   0.0;
		double b2 =  -sin(w0)/2.0;
		double a0 =   1.0 + alpha;
		double a1 =  -2.0*cos(w0);
		double a2 =   1.0 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as band-pass */
	void setBandPass( const Frequency freq, const float octaves, SampleRate sRate ) {
		double freqFact = double(freq) / double(sRate);
		setBandPass(freqFact, octaves);
	}


	/** configure the filter as all-pass. freqFact between ]0;0.5[ */
	void setAllPass( double freqFact, const float octaves ) {

		double w0 = 2.0 * K_PI * freqFact;
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =   1 - alpha;
		double b1 =  -2*cos(w0);
		double b2 =   1 + alpha;
		double a0 =   1 + alpha;
		double a1 =  -2*cos(w0);
		double a2 =   1 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as all-pass */
	void setAllPass( const Frequency freq, const float octaves, const SampleRate sRate ) {
		double freqFact = double(freq) / double(sRate);
		setAllPass(freqFact, octaves);
	}

	/** configure as notch filter. freqFact between ]0;0.5[ */
	void setNotch( double freqFact, const float octaves ) {

		double w0 = 2.0 * K_PI * freqFact;
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =   1.0;
		double b1 =  -2.0*cos(w0);
		double b2 =   1.0;
		double a0 =   1.0 + alpha;
		double a1 =  -2.0*cos(w0);
		double a2 =   1.0 - alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure as notch filter */
	void setNotch( const Frequency freq, const float octaves, const SampleRate sRate ) {
		double freqFact = double(freq) / double(sRate);
		setNotch(freqFact, octaves);
	}

	/** configure the filter as low-shelf. increase all aplitudes below freq? freqFact between ]0;0.5[ */
	void setLowShelf( double freqFact, const float octaves, const float gain ) {

		double A = sqrt( pow(10, (gain/20.0)) );
		double w0 = 2.0 * K_PI * freqFact;
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =     A*( (A+1.0) - (A-1.0)*cos(w0) + 2.0*sqrt(A)*alpha );
		double b1 = 2.0*A*( (A-1.0) - (A+1.0)*cos(w0)                   );
		double b2 =     A*( (A+1.0) - (A-1.0)*cos(w0) - 2.0*sqrt(A)*alpha );
		double a0 =         (A+1.0) + (A-1.0)*cos(w0) + 2.0*sqrt(A)*alpha;
		double a1 =  -2.0*( (A-1.0) + (A+1.0)*cos(w0)                   );
		double a2 =         (A+1.0) + (A-1.0)*cos(w0) - 2.0*sqrt(A)*alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}

	/** configure the filter as low-shelf. increase all aplitudes below freq? */
	void setLowShelf( const Frequency freq, const float octaves, const float gain, const SampleRate sRate ) {
		double freqFact = double(freq) / double(sRate);
		setLowShelf(freqFact, octaves, gain);
	}

	/** configure the filter as high-shelf. increase all amplitues above freq? freqFact between ]0;0.5[ */
	void setHighShelf( double freqFact, const float octaves, const float gain ) {

		double A = sqrt( pow(10, (gain/20.0)) );
		double w0 = 2.0 * K_PI * freqFact;
		double alpha = sin(w0)*sinh( log(2)/2 * octaves * w0/sin(w0) );

		double b0 =      A*( (A+1.0) + (A-1.0)*cos(w0) + 2.0*sqrt(A)*alpha );
		double b1 = -2.0*A*( (A-1.0) + (A+1.0)*cos(w0)                   );
		double b2 =      A*( (A+1.0) + (A-1.0)*cos(w0) - 2.0*sqrt(A)*alpha );
		double a0 =          (A+1.0) - (A-1.0)*cos(w0) + 2.0*sqrt(A)*alpha;
		double a1 =    2.0*( (A-1.0) - (A+1.0)*cos(w0)                   );
		double a2 =          (A+1.0) - (A-1.0)*cos(w0) - 2.0*sqrt(A)*alpha;

		setValues(a0, a1, a2, b0, b1, b2);

	}


	/** configure the filter as high-shelf. increase all amplitues above freq? */
	void setHighShelf( const Frequency freq, const float octaves, const float gain, const SampleRate sRate ) {
		double freqFact = double(freq) / double(sRate);
		setHighShelf(freqFact, octaves, gain);
	}


protected:

	/** pre-calculate the quotients for the filtering */
	void setValues(double a0, double a1, double a2, double b0, double b1, double b2) {
		b0a0 = float(b0/a0);
		b1a0 = float(b1/a0);
		b2a0 = float(b2/a0);
		a2a0 = float(a2/a0);
		a1a0 = float(a1/a0);
	}

	/** the bi-quad filter params */
	float b0a0;
	float b1a0;
	float b2a0;

	float a1a0;
	float a2a0;

	/** history for input values, per channel */
	Amplitude in[channels][2];

	/** history for ouput values, per channel */
	Amplitude out[channels][2];

	/** filter disabled due to wrong params? */
	bool disabled;

};

#endif // BIQUADFILTERGATE_H
