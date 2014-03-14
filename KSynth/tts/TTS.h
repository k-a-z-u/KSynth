#ifndef TTS_H
#define TTS_H

#include "flite/flite.h"

#include "../sampler/Sample.h"
#include "../misc/DataTypes.h"
#include "TTSProviderESpeak.h"


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
		return prov.speak(str);
	}


private:

	/** ctor */
	TTS() {

	}

	/** hidden copy ctor */
	TTS(const TTS&);

	/** hidden assignment op */
	TTS& operator == (const TTS&);


	TTSProviderESpeak prov;

};




#endif // TTS_H
