/*
 * MidiEvent.h
 *
 *  Created on: Nov 2, 2013
 *      Author: kazu
 */

#ifndef MIDIEVENT_H_
#define MIDIEVENT_H_

#include <sstream>
#include <vector>

#include "../misc/DataTypes.h"

/**
 * all possible midi-events as enum
 */
enum class MidiEventType {
	NOTE_OFF,
	NOTE_ON,
	POLYPHONIC_KEY_PRESSURE,
	CONTROL_CHANGE,
	PROGRAM_CHANGE,
	CHANNEL_PRESSURE,
	PITCH_WHEEL_CHANGE,
	META,
};

/**
 * represents one midi event with its
 *  delay-time
 *  status-byte
 * 	1st data-byte
 *  [2nd data-byte]
 */
struct MidiEvent {

	/** delay to the previous event on the same track */
	uint32_t delay;

	/** the status-byte */
	uint8_t status;

	/** the first data-byte */
	uint8_t d1;

	/** the second data-byte (if any) */
	uint8_t d2;


	/** empty ctor */
	MidiEvent() :
		delay(0), status(0), d1(0), d2(0) {;}

	/**
	 * ctor from given values
	 * @param delay the delay (to previous event) in milliseconds
	 * @param status the status byte
	 * @param d1 the first data byte
	 * @param d2 the second data byte
	 * */
	MidiEvent(uint32_t delay, uint8_t status, uint8_t d1, uint8_t d2) :
		delay(delay), status(status), d1(d1), d2(d2) {;}

	/**
	 * ctor from given byte-array (length 3)
	 * @param delay the delay (to previous event) in milliseconds
	 * @param data a byte array of length 3 (status, d1, d2)
	 */
	MidiEvent(uint32_t delay, uint8_t* data) :
		delay(delay), status(data[0]), d1(data[1]), d2 (data[2]) {;}

	/** get the midi event type behind the status byte */
	MidiEventType getType() const {
		uint8_t idx = (status >> 4) & 7; // 0b0111;
		return (MidiEventType)idx;
	}

	/** get the status-byte (type + channel) */
	uint8_t getStatus() const {
		return status;
	}

	/** get the first data-value */
	int getData1() const {
		return d1;
	}

	/** set the first data value */
	void setData1(int d1) {
		this->d1 = uint8_t(d1);
	}

	/** get the second data-value */
	int getData2() const {
		return d2;
	}

	/** set the second data-value */
	void setData2(int d2) {
		this->d2 = uint8_t(d2);
	}

	/** get the time-delay for this event */
	uint32_t getDelay() const {
		return delay;
	}

	/** set the time-delay for this event */
	void setDelay(uint32_t delay) {
		this->delay = delay;
	}

	/** set the midi event type within the status byte */
	void setType(MidiEventType t) {
		status = uint8_t( (status & 15) | 128 | (int(t) << 4) );		//15 = 0b1111	128 = 0b10000000
	}

	/** get the channel behind the status byte */
	uint8_t getChannel() const {
		return status & 15;		// 15 = 0b1111
	}


	/** textual display of a midi-event */
	std::string asString() const {
		std::stringstream ss;
		ss << "MidiEvent: ";
		ss << "channel(" << (int) getChannel() << ")\t";
		ss << "type(" << (int)status << ") = ";
		switch(getType()) {
			case MidiEventType::NOTE_OFF:					ss << "NOTE_OFF\tnote: " << (int)d1 << "\tvol: " << (int)d2; break;
			case MidiEventType::NOTE_ON:					ss << "NOTE_ON\tnote: " << (int)d1 << "\tvol: " << (int)d2; break;
			case MidiEventType::POLYPHONIC_KEY_PRESSURE:	ss << "POLYPHONIC_KEY_PRESSURE"; break;
			case MidiEventType::CONTROL_CHANGE:				ss << "CONTROL_CHANGE\td1: " << (int)d1 << "\td2: " << (int)d2; break;
			case MidiEventType::PROGRAM_CHANGE:				ss << "PROGRAM_CHANGE"; break;
			case MidiEventType::CHANNEL_PRESSURE:			ss << "CHANNEL_PRESSURE"; break;
			case MidiEventType::PITCH_WHEEL_CHANGE:			ss << "PITCH_WHEEL_CHANGE"; break;
			default: ss << "UNKNOWN"; break;
		};
		ss << "\t";
		return ss.str();
	}

	/** convert to byte vector */
	std::vector<unsigned char> asVector() const {
		std::vector<unsigned char> vec;
		vec.push_back(status);
		vec.push_back(d1);
		vec.push_back(d2);
		return vec;
	}

	/** equals? */
	bool operator == (const MidiEvent& other) const {
		return memcmp(this, &other, sizeof(MidiEvent)) == 0;
	}

};


/**
 * convenience wrapper for note-on events
 */
struct MidiEventNoteOn : public MidiEvent {

	unsigned int getNote() {return d1;}
	float getVolume() {return d2 / (Volume) 127;}

};


#endif /* MIDIEVENT_H_ */
