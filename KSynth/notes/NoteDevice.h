/*
 * NoteDevice.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef NOTEDEVICE_H_
#define NOTEDEVICE_H_

#include "../misc/DataTypes.h"
#include "Note.h"
#include "../misc/DeviceDetails.h"

/**
 * interface for all devices that my receive notes via
 * note-on and note-off events.
 */
class NoteDevice : virtual public DeviceDetails {

public:

	/** dtor */
	virtual ~NoteDevice() {;}

	/** start playing the given note at the given time */
	virtual void startNote(Note note, Volume volume) = 0;

	/** stop playing the given note */
	virtual void stopNote(Note note) = 0;

	/**
	 * stop all currently configured notes
	 * = reset the device
	 */
	virtual void stopNotes() = 0;

};


#endif /* NOTEDEVICE_H_ */
