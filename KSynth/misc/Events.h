/*
 * Events.h
 *
 *  Created on: Dec 6, 2013
 *      Author: kazu
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include <cstdint>

/**
 * all available types of events.
 * currently only midi is supported
 */
enum class EventType {
	MIDI,
};

/**
 * event that can be passed to a device in order
 * to change its behavior, changes parameters, enable/disable
 * notes, etc..
 *
 * for inheritance, other events will be casted to / from
 * this base-structure
 *
 * (inspired by VstEvent)
 */
struct Event {

	/** the event's type */
	EventType type;

	/** number of bytes in the data section */
	uint8_t size;

	/** the data section */
	uint8_t* data;

};


/**
 * combine several events to one call.
 *
 * (inspired by VstEvents)
 */
struct Events {

	/** the number of contained events */
	uint16_t numEvents;

	/** array of contained events */
	Event* events;

};




#endif /* EVENTS_H_ */
