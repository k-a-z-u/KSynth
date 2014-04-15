/*
 * FFTAnalyzer.h
 *
 *  Created on: Nov 3, 2013
 *      Author: kazu
 */

#ifndef FFTANALYZER_H_
#define FFTANALYZER_H_

#include <KLib/math/dsp/dft/FFT.h>
#include <KLib/math/dsp/window/DSPWindowBlackman.h>

/**
 * print spectrum analysis of the
 * input data
 */
class FFTAnalyzer {

#define FFT_SIZE	1024

public:

	/** ctor */
	FFTAnalyzer(SampleRate sRate) : sRate(sRate), fft(FFT_SIZE), header(0) {
		in.resize(FFT_SIZE);
		fftComplex.resize(FFT_SIZE);
		fftResult.resize(FFT_SIZE/2+1);
		fft.resize(FFT_SIZE);
	}

	/** dtor */
	~FFTAnalyzer() {
		;
	}

	void push(Amplitude a) {
		if (header < FFT_SIZE) {
			in[header] = a;
			++header;
		}
	}

	/** get values as vector */
	std::vector<float> get() {

		// apply windowing function to improve the FFT result
		window.apply( in.data() );

		// get fft
		fft.getComplexFFT( in.data(), fftComplex.data() );

		// adjust values
		for (unsigned int i = 0; i < FFT_SIZE/2+1; ++i) {
			fftResult[i] = std::sqrt( std::abs( fftComplex[i].real() * fftComplex[i].real() ) );
		}

		// re-enable adding and return result
		memset(in.data(), 0, sizeof(float)*FFT_SIZE);
		header = 0;
		return fftResult;

	}



private:

	/** the size to calculate the FFT over */
	SampleRate sRate;

	K::FFT fft;
	std::vector<float> in;
	std::vector<K::Complex<float>> fftComplex;
	std::vector<float>fftResult;
	K::DSPWindowBlackman<float, FFT_SIZE> window;

	unsigned int header;

};

#endif /* FFTANALYZER_H_ */
