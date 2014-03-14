#ifndef TTSPROVIDERFLITE_H
#define TTSPROVIDERFLITE_H

#ifdef WITH_TTS_FLITE

#include "TTSProvider.h"


// forward declaration
extern "C" {
	cst_voice* register_cmu_us_kal16();
}


/**
 * this TTS provider uses the FLite engine:
 * http://www.festvox.org/flite/
 *
 */
class TTSProviderFile : public TTSProvider {

public:

	/** ctor */
	TTSProviderFile() {

		// setup
		flite_init();
		voice = register_cmu_us_kal16();

	}

	Sample speak(const std::string& txt) override {

		// perform tts
		cst_wave* wave;
		wave = flite_text_to_wave(str.c_str(), voice);

		// convert short to float
		std::vector<Amplitude> pcm;
		pcm.resize(wave->num_samples);
		for (unsigned int i = 0; i < (unsigned int) wave->num_samples; ++i) {
			pcm[i] = Amplitude(wave->samples[i]) / Amplitude(6536) + Amplitude(0.5);
		}

		// construct sample
		return Sample(pcm, wave->sample_rate, wave->num_channels);

	}

private:

	/** the TTS voice */
	cst_voice* voice;

};

#endif

#endif // TTSPROVIDERFLITE_H
