#ifndef SOUNDSINKHARDWAREALSA_H_
#define SOUNDSINKHARDWAREALSA_H_

#ifdef WITH_ALSA

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <exception>
#include <string>
#include <iostream>
#include <memory>

#include "SoundSinkHardware.h"
#include "../misc/AudioFormat.h"

/** exception handling within alsa backend */
class SoundSinkHardwareAlsaException : public SoundSinkException {
public:
	SoundSinkHardwareAlsaException(const std::string& str) : SoundSinkException(str) {;}
	static SoundSinkHardwareAlsaException getFromAlsaError(const std::string& str, long int err) {
		return SoundSinkHardwareAlsaException("ALSA Error:\n" + str + "\n" + "error: " + snd_strerror((int)err));
	}
};


/**
 * A SoundSink using linux' ALSA audio output.
 *
 * Sinks cannot be created directly but via fetching a list
 * of all available sound devices.
 *
 */
class SoundSinkHardwareAlsa : public SoundSinkHardware {

public:

	/** dtor */
	virtual ~SoundSinkHardwareAlsa() {
		close();
	}

	void open(AudioFormat fmt) override {
		init(fmt);
	}

	void push(const Amplitude** outputs, const SampleFrame frames) override {

		long int err;
		if ((err = snd_pcm_writen (playback_handle, (void**) outputs, frames)) != frames) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("writing to audio interface failed", err);
		}

	}

	void finalize() override {
		close();
	}

	std::string getName() override {
		return "ALSA: " + name;
	}


	/** get all available alsa devices */
	static std::vector< std::unique_ptr<SoundSinkHardwareAlsa> > getAllDevices() {

		std::vector< std::unique_ptr<SoundSinkHardwareAlsa> > vec;

		char** hints;
		int err = snd_device_name_hint(-1, "pcm", (void***) &hints );
		if (err != 0) {return vec;}

		// enumerate devices
		char** n = hints;
		while (*n != nullptr) {

			char* name = snd_device_name_get_hint(*n, "NAME");

			if (name != NULL && 0 != strcmp("null", name)) {

				// create new sink
				std::unique_ptr<SoundSinkHardwareAlsa> ptr(new SoundSinkHardwareAlsa(name));
				vec.push_back( std::move(ptr) );

				// cleanup
				free(name);

			}

			++n;

		}

		// cleanup
		snd_device_name_free_hint((void**)hints);

		return vec;

	}

	int getLatencyMS() override {
		return -1;
	}

private:

	/** hidden ctor */
	SoundSinkHardwareAlsa(const std::string& name) :
		name(name), playback_handle(nullptr) {
		;
	}


	/** initialize the sound-card for using the given audio format */
	void init(AudioFormat fmt) {

		int err;
		const char* dev = name.c_str();//"default";

		snd_pcm_hw_params_t *hw_params;

		if ((err = snd_pcm_open (&playback_handle, dev, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("error while opening device", err);
		}

		if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("cannot allocate hardware parameter structure", err);
		}

		if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("cannot initialize hardware parameter structure", err);
		}

		if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_NONINTERLEAVED)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("cannot set access type", err);
		}

		if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_FLOAT)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("cannot set sample format", err);
		}

		if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &fmt.sampleRate, 0)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("cannot set sample rate", err);
		}

		if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, fmt.numChannels)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("cannot set channel count", err);
		}

		if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("cannot set parameters", err);
		}

		snd_pcm_hw_params_free (hw_params);

		if ((err = snd_pcm_prepare (playback_handle)) < 0) {
			throw SoundSinkHardwareAlsaException::getFromAlsaError("cannot prepare audio interface for use", err);
		}

	}

	/** close soundcard */
	void close() {
		if (!playback_handle) {return;}
		snd_pcm_hw_free (playback_handle);
		snd_pcm_close (playback_handle);
		playback_handle = nullptr;
	}



private:

	/** the device's name */
	std::string name;

	int bufIdx = 0;

	snd_pcm_t *playback_handle;

	AudioFormat fmt;



};

#endif

#endif /* SOUNDSINKHARDWAREALSA_H_ */
