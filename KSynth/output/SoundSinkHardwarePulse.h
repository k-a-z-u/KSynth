#ifndef SOUNDSINKHARDWAREPULSE_H_
#define SOUNDSINKHARDWAREPULSE_H_

#ifdef WITH_PULSE_AUDIO


#include <pulse/simple.h>
#include <pulse/error.h>


#include "SoundSinkHardware.h"
#include "../misc/AudioFormat.h"


/** exception handling within pulseaudio backend */
class SoundSinkHardwarePulseException : public SoundSinkException {
public:
	SoundSinkHardwarePulseException(const std::string& str) : SoundSinkException(str) {;}
	static SoundSinkHardwarePulseException getFromPulseError(const std::string& str, long int err) {
		return SoundSinkHardwarePulseException("Pulse Error:\n" + str + "\n" + "error: " + pa_strerror((int)err));
	}
};


/**
 * A SoundSink using linux' PulseAudio output.
 *
 */
class SoundSinkHardwarePulse : public SoundSinkHardware {

public:

	/** dtor */
	virtual ~SoundSinkHardwarePulse() {
		close();
	}

	void open(AudioFormat fmt) override {
		this->fmt = fmt;
		this->ss.format = PA_SAMPLE_FLOAT32LE;
		this->ss.channels = fmt.numChannels;
		this->ss.rate = fmt.sampleRate;

		int err = 0;
		s = pa_simple_new(NULL, "KSynth", PA_STREAM_PLAYBACK, NULL, "Synthesizer", &this->ss, NULL, NULL, &err);
		if(err != 0){
			throw SoundSinkHardwarePulseException::getFromPulseError("error while opening device", err);
		}
	}

	void push(const Amplitude** outputs, const SampleFrame frames) override {
		int err = 0;

		Amplitude* buffer = new Amplitude[this->fmt.numChannels * frames];
		unsigned int cnt = 0;

		// interleave audio data
		for (unsigned int i = 0; i < frames; ++i) {
			for(unsigned int c = 0; c < this->fmt.numChannels; ++c){
				buffer[cnt++] = outputs[c][i];
			}
		}

		try{
			size_t bytes = this->fmt.numChannels * frames * sizeof(float);
			pa_simple_write(this->s, buffer, bytes, &err);
			if(err != 0){
				throw SoundSinkHardwarePulseException::getFromPulseError("writing to audio interface failed", err);
			}
			pa_simple_drain(this->s, &err);
			if(err != 0){
				throw SoundSinkHardwarePulseException::getFromPulseError("writing to audio interface failed", err);
			}
		}catch(SoundSinkHardwarePulseException ex){
			//cleanup
			delete buffer;
			throw ex;
		}
	}

	void finalize() override {
		close();
	}

	std::string getName() override {
		return "PulseAudio Output";
	}


	/** get all available pulse output sinks */
	static std::vector< std::unique_ptr<SoundSinkHardwarePulse> > getAllDevices() {
		return std::vector< std::unique_ptr<SoundSinkHardwarePulse> >();
	}

	int getLatencyMS() override {
//		int err;
//		pa_usec_t ms = pa_simple_get_latency(s, &err);
//		return (err == 0) ? (int)ms : -1;
		return -1;
	}

private:

	/** close connection */
	void close() {
		if(this->s != nullptr){
			pa_simple_free(this->s);
			this->s = nullptr;
		}
	}


private:

	/** pulseaudio session */
	pa_simple *s;
	pa_sample_spec ss;

	AudioFormat fmt;

};

#endif

#endif /* SOUNDSINKHARDWAREPULSE_H_ */
