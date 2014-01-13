/*
 * MidiDeviceLaunchkeyMini.h
 *
 *  Created on: Dec 27, 2013
 *      Author: kazu
 */

#ifndef MIDIDEVICELAUNCHKEYMINI_H_
#define MIDIDEVICELAUNCHKEYMINI_H_

#include "../RtMidiOutDevice.h"

/**
 * enum for LED-brightness values
 */
enum class MidiDeviceLaunckeyMiniBrightness {
	OFF,
	LOW,
	MEDIUM,
	HIGH
};

/**
 * enum values for color selection
 */
enum class MidiDeviceLaunckeyMiniColor {

	OFF = 0,

	RED_LOW = 1,
	RED_MEDIUM = 2,
	RED_HIGH = 3,

	GREEN_LOW = 16,
	GREEN_MEDIUM = 32,
	GREEN_HIGH = 48,


};

enum class MidiDeviceLaunckeyMiniLED {
	MIDI_CHANNEL = 12,
			LED_1 = 96,
			LED_2 = 97,
			LED_3,
			LED_4,
			LED_5,
			LED_6,
			LED_7,
			LED_8,
			LED_9,
			LED_10,
			LED_11,
			LED_12,
			LED_13,
			LED_14,
			LED_15,
			ROUND_UP,
			ROUND_DOWN,
};

/**
 * helper class for a Novation Launchkey Mini to control its LEDs
 *
 * for reference PDF see:
 * http://www.novationmusic.com/download/668/
 *
 */
class MidiDeviceLaunckeyMini {

public:

	/** ctor */
	MidiDeviceLaunckeyMini(RtMidiOutDevice& dev) : dev(dev) {
		dev.open();
	}

	/**
	 * trigger a LED test, turning on all LEDs using a given brightness
	 */
	void setLEDTest(MidiDeviceLaunckeyMiniBrightness brightness) {
		MidiEvent evt(0, 176, 0, uint8_t( 125 - 1 + int(brightness) ));
		dev.sendEvent(evt);
	}

	void reset() {
		MidiEvent evt(0, 176, 0, 0);
		dev.sendEvent(evt);
	}

	void setLED(unsigned int x, unsigned int y, int val) {
		unsigned int idx = 96 + y*16 + x;
		MidiEvent evt(0, 144, uint8_t(idx), uint8_t(val));
		dev.sendEvent(evt);
	}

	void setLED(unsigned int x, unsigned int y, MidiDeviceLaunckeyMiniColor color) {
		setLED(x,y, (int) color);
	}

	void setLED(MidiDeviceLaunckeyMiniLED led, int val) {
		MidiEvent evt(0, 144, uint8_t(led), uint8_t(val));
		dev.sendEvent(evt);
	}



private:

	/** the device to write to */
	RtMidiOutDevice& dev;

};


#endif /* MIDIDEVLAUNCHKEYMINI_H_ */
