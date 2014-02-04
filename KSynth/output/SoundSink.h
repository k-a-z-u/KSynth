#ifndef SOUNDSINK_H_
#define SOUNDSINK_H_

#include "../misc/DataTypes.h"
#include "../misc/AudioFormat.h"

/** convert amplitude [-1,+1] to int16_t */
#define amplitudeToI16(val)		uint16_t(  ((val / 2.0) * ((1 << 16) - 1))  )


/** exception handling within SoundSinks */
class SoundSinkException : public std::exception {
public:
	SoundSinkException(const std::string& str) : str(str) {;}
	virtual const char* what() const throw() { return str.c_str(); }
private:
	std::string str;
};


/**
 * generated audio data is piped into a SoundSink which
 * sends the data to e.g. a soundcard, wave-file, mp3, etc...
 *
 * this is the interface such audio targets must fulfill.
 */
class SoundSink {

public:

	/** dtor */
	virtual ~SoundSink() {;}


	/**
	 * open the underlying hardware, exporter, etc and configure
	 * it for using the provided AudioFormat (samplerate, channels, ..)
	 * @param fmt the format to use
	 */
	virtual void open(AudioFormat fmt) = 0;

	/**
	 * append this block of audio-data to the output.
	 * (send to audio-hardware, encode to mp3, ..)
	 * @param channels an array containing all channels and their amplitudes
	 * @param samples the number of samples (amplitudes) per channel
	 */
	virtual void push(const Amplitude** channels, const SampleFrame samples) = 0;

	/**
	 * finalize (close) the sink.
	 * disconnect from hardware, write file headers, etc.
	 *
	 * after calling finalize it must be possible to open()
	 * the device again.
	 */
	virtual void finalize() = 0;

	/**
	 * get a name describing this audio-sink
	 */
	virtual std::string getName() = 0;


};


#endif /* SOUNDSINK_H_ */
