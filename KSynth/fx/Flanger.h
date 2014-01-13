/*
 * Flanger.h
 *
 *  Created on: Oct 31, 2013
 *      Author: kazu
 */

#ifndef FLANGER_H_
#define FLANGER_H_


#include "../misc/SoundSource.h"
#include "../oscillator/SimpleOscillator.h"

/**
 * a simple flanger implementation
 */
class Flanger : public SoundProcessor {

public:

	/** ctor */
	Flanger(AudioFormat fmt) : SoundProcessor(2,2), delay(fmt), maxDelay(0) {
		;
		osc.setMode(SimpleOscillatorMode::SINE);
		setFrequency(1.0);
		setMaxDelay(0.002);
	}

//	AudioData get(Time t) override {
//		delay.setDelay(maxDelay * (osc.get(t)+1.0));
//		AudioData aDelay = delay.get(t);
//		AudioData oOrig = delay.getLatest();
//		//aDelay *= 0.5;
//		//oOrig *= 0.5;
//		aDelay += oOrig;
//		aDelay *= 0.75;
//		return aDelay;
//	}

	void process(Amplitude** input, Amplitude** output) override {
		// TODO
	}

	/** set the input source for the delay filter */
	void setSource(SoundSource* src) {
		delay.setSource(src);
	}

	/** set the frequency to use for the flanger */
	void setFrequency(Frequency freq) {
		osc.setFrequency(freq);
	}

	/** set the maximum delay (the impact) for the flanger */
	void setMaxDelay(Time delay) {
		delay *= 0.5;								// see get(Time t). faster!
		maxDelay = (delay < 1) ? (delay) : (1);
	}

private:

	/** the delay to use for the flanger */
	Delay delay;

	/** the oscillator for the flanger */
	SimpleOscillator osc;

	/** the maximum delay to use */
	Time maxDelay;

};


#endif /* FLANGER_H_ */
