/*
 * RtMidiDevice.h
 *
 *  Created on: Dec 27, 2013
 *      Author: kazu
 */

#ifndef RTMIDIDEVICE_H_
#define RTMIDIDEVICE_H_


enum class RtMidiDeviceType {
	INPUT,
	OUTPUT,
};

/**
 * wrapper class for a Midi-device identified by an
 * ID or a given name
 */
class RtMidiDevice {


public:

	/** dtor */
	virtual ~RtMidiDevice() {;}

	/** get the name of this midi device */
	const std::string& getName() {
		return name;
	}

	/** get the device's index */
	unsigned int getDeviceIndex() {
		return id;
	}

	virtual RtMidiDeviceType getDeviceType() = 0;

protected:


	/** the rt-midi id */
	unsigned int id;

	/** the device's name */
	std::string name;

};


#endif /* RTMIDIINDEVICE_H_ */
