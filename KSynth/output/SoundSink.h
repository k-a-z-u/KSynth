/*
 * SoundSink.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef SOUNDSINK_H_
#define SOUNDSINK_H_

#include "../misc/DataTypes.h"

/** convert amplitude [-1,+1] to int16_t */
#define amplitudeToI16(val)		((val / 2.0) * ((1 << 16) - 1))


/** exception handling within SoundSinks */
class SoundSinkException : public std::exception {
public:
	SoundSinkException(const std::string& str) : str(str) {;}
	virtual const char* what() const throw() { return str.c_str(); }
private:
	std::string str;
};


/**
 * the generated audio data will be piped into a SoundSink which
 * sends the data to e.g. a soundcard, wave-file, mp3, etc...
 */
class SoundSink {

public:

	/** dtor */
	virtual ~SoundSink() {;}


	/** open the sound sink */
	virtual void open() = 0;

	/** append data to the sink */
	virtual void push(const Amplitude** channels, const SampleFrame samples) = 0;

	/** finalize (close) the sink */
	virtual void finalize() = 0;


};


#endif /* SOUNDSINK_H_ */
