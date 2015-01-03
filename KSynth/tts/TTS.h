#ifndef TTS_H
#define TTS_H

#include "../sampler/Sample.h"
#include "../misc/DataTypes.h"
#include "TTSProvider.h"
#include "TTSProviderESpeak.h"
#include "TTSProviderFlite.h"


/**
 * perform TTS synthesis using FLite.
 */
class TTS {

public:

	/** singleton access */
	static TTS& get() {
		static TTS tts;
		return tts;
	}

	/** get sample for the given string */
	Sample toSample(const std::string& str) {
		if(prov != nullptr){return Sample();}
		return prov->speak(str);
	}


private:

	/** ctor */
	TTS() {
#ifdef WITH_TTS_ESPEAK
		this->prov = (TTSProvider)new TTSProviderESpeak();
#endif
#ifdef WITH_TTS_FLITE
		this->prov = (TTSProvider)new TTSProviderFlite();
#endif
	}

	/** hidden copy ctor */
	TTS(const TTS&);

	/** hidden assignment op */
	TTS& operator == (const TTS&);

	TTSProvider* prov = nullptr;

};




#endif // TTS_H
