/*
 * SoundSinkAlsa.h
 *
 *  Created on: Oct 23, 2013
 *      Author: kazu
 */

#ifndef SOUNDSINKALSA_H_
#define SOUNDSINKALSA_H_

#ifdef WITH_ALSA

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <exception>
#include <string>
#include <iostream>
#include "SoundSink.h"
#include "../misc/AudioFormat.h"

/** exception handling within alsa backend */
class SoundSinkAlsaException : public SoundSinkException {
public:
	SoundSinkAlsaException(const std::string& str) : SoundSinkException(str) {;}
	static SoundSinkAlsaException getFromAlsaError(const std::string& str, long int err) {
		return SoundSinkAlsaException("ALSA Error:\n" + str + "\n" + "error: " + snd_strerror((int)err));
	}
};


/**
 * A SoundSink sending all it's data to the primary alsa soundcard
 */
class SoundSinkAlsa : public SoundSink {

public:

	SoundSinkAlsa(AudioFormat fmt) : playback_handle(nullptr), fmt(fmt) {
		//init(fmt);
	}

	virtual ~SoundSinkAlsa() {
		close();
	}

	void open() override {
		init(fmt);
	}

//	/** play the given buffer */
//	void push(short* buf, int samples) {
//		long int err;
//		int frames = samples / fmt.numChannels;
//		if ((err = snd_pcm_writei (playback_handle, buf, frames)) != frames) {
//			//snd_pcm_recover(playback_handle, err, 0);
//			throw SoundException::getFromAlsaError("write to audio interface failed", err);
//		}
//		//std::cout << "snd flush" << std::endl;
//	}

	/** attach one value. flushes when buffer is full */
	void push(const Amplitude** outputs, const SampleFrame frames) override {
//		short sLeft = short ( (a.left / 2) * ((1 << 16) - 1) );
//		short sRight = short ( (a.right / 2) * ((1 << 16) - 1) );
//		if (bufIdx >= ALSA_BUF_SIZE) {push(buf, ALSA_BUF_SIZE); bufIdx = 0;}
//		buf[bufIdx+0] = sLeft;
//		buf[bufIdx+1] = sRight;
//		bufIdx += fmt.numChannels;

		// create interleaved output
//		unsigned int sizePerChannel = frames * (unsigned int) sizeof(Amplitude);
//		Amplitude* output = (Amplitude*) malloc( sizePerChannel * 2 );
//		memcpy(output + 0, outputs[0], sizePerChannel);
//		memcpy(output + frames, outputs[1], sizePerChannel);

		long int err;
		if ((err = snd_pcm_writen (playback_handle, (void**) outputs, frames)) != frames) {
//			free(output);
			throw SoundSinkAlsaException::getFromAlsaError("write to audio interface failed", err);
		}

//		free(output);

	}

	void finalize() override {
		close();
	}


private:

	/** init the soundcard */
	void init(AudioFormat fmt) {

		int err;
		const char* dev = "default";

		snd_pcm_hw_params_t *hw_params;

		if ((err = snd_pcm_open (&playback_handle, dev, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("error while opening device", err);
		}

		if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("cannot allocate hardware parameter structure", err);
		}

		if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("cannot initialize hardware parameter structure", err);
		}

		if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_NONINTERLEAVED)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("cannot set access type", err);
		}

		if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_FLOAT)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("cannot set sample format", err);
		}

		if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &fmt.sampleRate, 0)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("cannot set sample rate", err);
		}

		if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, fmt.numChannels)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("cannot set channel count", err);
		}

		if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("cannot set parameters", err);
		}

		snd_pcm_hw_params_free (hw_params);

		if ((err = snd_pcm_prepare (playback_handle)) < 0) {
			throw SoundSinkAlsaException::getFromAlsaError("cannot prepare audio interface for use", err);
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

	int bufIdx = 0;
	//short buf[ALSA_BUF_SIZE];
	snd_pcm_t *playback_handle;
	AudioFormat fmt;



};

#endif

#endif /* SOUNDSINKALSA_H_ */
