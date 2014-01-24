#ifndef SYSTEMVERSION_H
#define SYSTEMVERSION_H

/**
 * provides information about the system version
 * and compiled-in modules
 */
class SystemVersion {

public:

	/** compiled with alsa support? */
	static bool hasALSA() {
#ifdef WITH_ALSA
		return true;
#else
		return false;
#endif
	}

	/** compiled with flac loading support? */
	static bool hasFLAC() {
#ifdef WITH_FLAC
		return true;
#else
		return false;
#endif
	}

	/** compiled with lame mp3 export? */
	static bool hasMP3Lame() {
#ifdef WITH_LAME
		return true;
#else
		return false;
#endif
	}

	/** compiled with PulseAudio support? */
	static bool hasPulseAudio() {
#ifdef WITH_PULSE_AUDIO
		return true;
#else
		return false;
#endif
	}

	/** compiled with fftw3 (spectrum analysis, ...) */
	static bool hasFFTW3() {
#ifdef WITH_FFTW3
		return true;
#else
		return false;
#endif
	}

	/** compiled with zlib support? */
	static bool hasZLIB() {
#ifdef WITH_ZLIB
		return true;
#else
		return false;
#endif
	}

	/** compiled with windows WaveOut support? */
	static bool hasWaveOut() {
#ifdef WITH_WAVE_OUT
		return true;
#else
		return false;
#endif
	}



};

#endif // SYSTEMVERSION_H
