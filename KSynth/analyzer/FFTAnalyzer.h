/*
 * FFTAnalyzer.h
 *
 *  Created on: Nov 3, 2013
 *      Author: kazu
 */

#ifndef FFTANALYZER_H_
#define FFTANALYZER_H_

#ifdef WITH_FFTW3

#include <fftw3.h>
//#include "../lib/gnuplot/Gnuplot.h"

/**
 * print spectrum analysis of the
 * input data
 */
class FFTAnalyzer {

public:

	/** ctor */
	FFTAnalyzer(SampleRate sRate) : sRate(sRate), size(sRate / 10), header(0) {

		// create the plan
		in = fftw_alloc_real(size);
		out = fftw_alloc_complex(size/2+1);
		plan = fftw_plan_dft_r2c_1d(size, in, out, FFTW_MEASURE | FFTW_PRESERVE_INPUT);

	}

	/** dtor */
	~FFTAnalyzer() {
		fftw_free(in);				in = nullptr;
		fftw_free(out);				out = nullptr;
		fftw_destroy_plan(plan);	plan = nullptr;
	}

	void push(Amplitude a) {
		in[header] = a;
		++header;
		if ( header == size ) {
			fftw_execute(plan);
			header = 0;
		}
	}

	/** get values as vector */
	std::vector<float> get() {
		std::vector<float> vec;
		for (unsigned int i = 0; i < size / 3; ++i) {vec.push_back( std::fabs(out[i][0]) );}
		return vec;
	}



private:

	/** the size to calculate the FFT over */
	SampleRate sRate;
	unsigned int size;

	/** the plan to use for calculating the FFT */
	fftw_plan plan;

	double* in;
	fftw_complex* out;

	unsigned int header;

};

#endif


#endif /* FFTANALYZER_H_ */
