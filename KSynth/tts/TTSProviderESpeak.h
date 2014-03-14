#ifndef TTSPROVIDERESPEAK_H
#define TTSPROVIDERESPEAK_H

#ifdef WITH_TTS_ESPEAK

#include "TTSProvider.h"

#include <espeak/speak_lib.h>

class TTSProviderESpeak : public TTSProvider {

public:

	/** ctor */
	TTSProviderESpeak() {

		const char* path = nullptr;
		const int options = 0;
		const int bufLen = 0;

		// export PCM only and block until speaking is done
		espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_SYNCHRONOUS;


		// setup
		hz = espeak_Initialize(output, bufLen, path, options );
		espeak_ERROR ret = espeak_SetVoiceByName( "default" );

		// check result
		if (hz == -1 || ret != EE_OK) {
			throw TTSProviderException("error while initializing ESpeak");
		}

		// register callback
		espeak_SetSynthCallback(&TTSProviderESpeak::callback);

	}

	/** dtor */
	~TTSProviderESpeak() {
		espeak_Terminate();
	}

	Sample speak(const std::string &text) override {

		// perform TTS
		pcm.clear();
		int synthFlags = espeakCHARS_AUTO | espeakPHONEMES | espeakSSML;
		espeak_ERROR ret = espeak_Synth(text.c_str(), text.length()+1, 0, POS_CHARACTER, 0, synthFlags, 0, this);

		// check result
		if (ret != EE_OK) {
			throw TTSProviderException("error while speaking text");
		}

		// done
		return Sample(pcm, hz, 1);

	}


private:

	static int callback(short* raw, int numSamples, espeak_EVENT* evt) {

		TTSProviderESpeak* prov = (TTSProviderESpeak*) evt->user_data;
		for (unsigned int i = 0; i < numSamples; ++i) {
			Amplitude a = Amplitude(raw[i]) / Amplitude(65536/2) * Amplitude(2);
			prov->pcm.push_back( a );
		}

		return 0;

	}


	/** rendered PCM data will go here */
	std::vector<Amplitude> pcm;

	/** the Hz rate */
	int hz;

};

#endif

#endif // TTSPROVIDERESPEAK_H
