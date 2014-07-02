#ifndef VOCODER_H
#define VOCODER_H

#include "../misc/SoundProcessor.h"
#include "../notes/Note.h"
#include "BiquadFilter.h"
#include <KLib/math/dsp/dft/FFT.h>
#include <KLib/math/dsp/window/DSPWindowBlackman.h>

#define VOC_BANDS 256

class Vocoder : public SoundProcessor {

public:

	/** ctor */
	Vocoder() : SoundProcessor(4,2), fft1(VOC_BANDS), fft2(VOC_BANDS), idx(0)  {

		// setup filters
		for (unsigned int i = 0; i < VOC_BANDS; ++i) {

			// setup input-filter band
			float freq = Note(67 + i).getFrequency();
			freqs[i] = freq;
			inFilters[i].setBandPass(freq, 0.015f, getSampleRate());

			in2Filters[i].setBandPass(freq, 0.015f, getSampleRate());

			// setup next stange envelope filter
			envFilters[i].setLowPass(32*VOC_BANDS/2, 1.0f, getSampleRate());
			envFiltersR[i].setLowPass(32*VOC_BANDS/2, 1.0f, getSampleRate());
			envFiltersI[i].setLowPass(32*VOC_BANDS/2, 1.0f, getSampleRate());

		}


	}


	void process(Amplitude** input, Amplitude** output) override {

		for (unsigned int i = 0; i < getSamplesPerProcess(); i += VOC_BANDS) {

			// FFT
			window.apply(&input[0][i]);
			window.apply(&input[2][i]);

			fft1.getComplexFFT(&input[0][i], cplx1);
			fft1.getComplexFFT(&input[2][i], cplx2);

			winComplex.apply(cplx1);


//			// MUL
//			for (unsigned int j = 0; j < VOC_BANDS; j += 2) {
//				//cplx1[j] *= cplx2[j];
//				//cplx1[j] = cplx1[(j-1+VOC_BANDS) % VOC_BANDS];
//				cplx1[j].imag(0);
//				cplx1[j] *= cplx2[j];
//			}

			for (unsigned int j = 0; j < VOC_BANDS; ++j) {
				//cplx1[j] = (cplx1[j] * 0.05f) + (cplx1b[j] * 0.95f);
				//cplx1b[j] = cplx1[j];

				//v1[j] = cplx1[j].mag() * 0.1f + v2[j] * 0.9f;
				//v2[j] = v1[j];
				//v1[j] = envFilters[j].filter( 0, cplx1[j].mag() );

				float env = envFilters[j].filter (0,cplx1[j].mag());
				//cplx1[j].r = envFiltersR[j].filter(0, cplx1[j].r);
				//cplx1[j].i = 0;//envFiltersI[j].filter(0, cplx1[j].i);
				//cplx1[j] = cplx2[j];// cplx1[j] + (cplx2[j] * env);//cplx1[j];
				cplx1[j] = cplx2[j] * env;

			}


//			for (unsigned int j = VOC_BANDS/2; j < VOC_BANDS; ++j) {
//				cplx1[j].r = 0;
//				cplx1[j].i = 0;
//			}

			// iFFT
			fft1.getIFFT(cplx1, &output[0][i]);
			memcpy(&output[1][i], &output[0][i], sizeof(float) * VOC_BANDS);

			/*
			// pass input through each filter band
			for (unsigned int k = 0; k < VOC_BANDS; ++k) {
				float out = 0;
				out += std::sin( 2 * 3.1415 * 200 * idx / getSampleRate() ) * v1[0] * 0.3;
				out += std::sin( 2 * 3.1415 * 400 * idx / getSampleRate() ) * v1[1];
				out += std::sin( 2 * 3.1415 * 600 * idx / getSampleRate() ) * v1[2];
				out += std::sin( 2 * 3.1415 * 800 * idx / getSampleRate() ) * v1[3];
				out += std::sin( 2 * 3.1415 * 1000 * idx / getSampleRate() ) * v1[4];
				out += std::sin( 2 * 3.1415 * 1200 * idx / getSampleRate() ) * v1[5];
				out += std::sin( 2 * 3.1415 * 1400 * idx / getSampleRate() ) * v1[6];
				out += std::sin( 2 * 3.1415 * 1800 * idx / getSampleRate() ) * v1[8];
				out += std::sin( 2 * 3.1415 * 2200 * idx / getSampleRate() ) * v1[10];
				out += std::sin( 2 * 3.1415 * 2600 * idx / getSampleRate() ) * v1[12];
				out += std::sin( 2 * 3.1415 * 3000 * idx / getSampleRate() ) * v1[14];
				out += std::sin( 2 * 3.1415 * 3400 * idx / getSampleRate() ) * v1[16];
				out += std::sin( 2 * 3.1415 * 3800 * idx / getSampleRate() ) * v1[18];
				//out += std::sin( 2 * 3.1415 * 1600 * idx / getSampleRate() ) * (cplx1[8].mag()+cplx1[9].mag());
				//out += std::sin( 2 * 3.1415 * 3200 * idx / getSampleRate() ) * (cplx1[16].mag()+cplx1[17].mag());
				output[0][k+i] = out * 0.2f;
				output[1][k+i] = out * 0.2f;
				++idx;
			}
			*/


		}

	}

	/*
	void process(Amplitude** input, Amplitude** output) override {

		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {

			Volume in1 = input[0][i];// + input[1][i] / 2.0f;
			Volume in2 = input[2][i];//(input[2][i] + input[3][i]) / 2.0f;
			Volume out = 0;

			// pass input through each filter band
			for (unsigned int j = 0; j < VOC_BANDS; ++j) {

				Volume v1 = in1;
				v1 = inFilters[j].filter(0, v1);
				v1 = std::fabs(v1);
				v1 = envFilters[j].filter(0, v1);

				//Volume v2 = in2;
				//v2 = in2Filters[j].filter(0, v2);

				//out += v1 * v2;
				//Volume v3 = (VOC_BANDS - j) / VOC_BANDS;

				// GOOD
				out += std::sin( 1.5 * 3.1415 * freqs[j]  * idx / getSampleRate() ) * v1;


			}

			out *= 0.2f;
			out = clamp1(out);
			//out += in1 * 0.1f;

			output[0][i] = out;
			output[1][i] = out;

			++idx;

		}

	}
	*/

	unsigned int getNumParameters() const override {
		return 1;
	}

	std::string getProductString() const override {
		return "Vocoder";
	}

private:

	K::FFT fft1;
	K::FFT fft2;

	K::DSPWindowBlackman<K::Complex<float>, VOC_BANDS/2> winComplex;
	K::DSPWindowBlackman<float, VOC_BANDS> window;

	K::Complex<float> cplx1[VOC_BANDS];
	K::Complex<float> cplx1b[VOC_BANDS];
	K::Complex<float> cplx2[VOC_BANDS];

	float v1[VOC_BANDS];
	float v2[VOC_BANDS];

	BiquadFilter envFiltersR[VOC_BANDS];
	BiquadFilter envFiltersI[VOC_BANDS];

	int idx;

	float freqs[VOC_BANDS];

	BiquadFilter inFilters[VOC_BANDS];

	BiquadFilter envFilters[VOC_BANDS];

	BiquadFilter in2Filters[VOC_BANDS];

};


#endif // VOCODER_H
