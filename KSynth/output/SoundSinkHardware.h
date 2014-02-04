#ifndef SOUNDSINKHARDWARE_H
#define SOUNDSINKHARDWARE_H

#include "SoundSink.h"

/**
 * hardware SoundSinks write piped audio data
 * to hardware-devices like sound-cards.
 *
 * those sinks provide additional methods for
 * e.g. retrieving the hardware's latency.
 */
class SoundSinkHardware : public SoundSink {

public:

	/** dtor */
	virtual ~SoundSinkHardware() {;}

	/**
	 * if applicable, get the latency (in milliseconds) of the underlying
	 * device. this is mainly intended for hardware sinks (sound-cards).
	 */
	virtual int getLatencyMS() {return -1;}


};

#endif // SOUNDSINKHARDWARE_H
