/*
 * Reverb.h
 *
 *  Created on: Oct 28, 2013
 *      Author: kazu
 */

#ifndef REVERB_H_
#define REVERB_H_

#include "Delay.h"

#include "../misc/VUCalc.h"
#include "BiquadFilter.h"
#include <iostream>

struct ReverbEntry {
	float percent;
	Volume volume;
	ReverbEntry(float percent, Volume volume) : percent(percent), volume(volume) {;}
};


///** pseudo random number between [-1,+1] */
//static float pseudoRand() {
//	static unsigned int nSeed = 5323;
//	nSeed = (8253729 * nSeed + 2396403);
//	return fmodf(nSeed / 32767.0f, 2.0f) - 1.0f;
//}

class Reverb : public SoundProcessor {

public:

	/** ctor */
	Reverb() :
		SoundProcessor(2,2),src(nullptr), size(0), dryWet(0.5), delay(getSampleRate()) {
		//bq.setBandPass(800, 5.5);
		//bq.reset();
	}

	void config(int cnt, Time delay, Volume dryWet) {

		std::vector<ReverbEntry> chain;
		this->delay.setDelayImmediately(delay);

		this->dryWet = dryWet;
		//float sSize = delay / cnt;

		Volume volSum = 0;

		// create an entry for each of the requested delays
		for (int i = 0; i < cnt; ++i) {

			// percent within total delay
			//	0% = oldest value			(complete delay for echo)
			//	100% = most recent value	(no echo)
			//	-> NEVER use 100% value as it will be added directly
			float percent = float(i) / float(cnt);

			// add some random factor.. sounds more realistic
			//percent += (sSize / 4.0) * pseudoRand();

			// the volume to use for this delay entry
			Volume vol = 0.1f + percent;

			// sum-up all volumes (for normalization)
			volSum += vol;

			// attach
			chain.push_back(ReverbEntry(1.0f-percent, vol));

		}

		// normalize reverb volume to match the dry/wet level
		for (int i = 0; i < cnt; ++i) {
			chain[i].volume /= volSum / (1.0f - dryWet);
			//std::cout << chain[i].percent << ":" << chain[i].volume << std::endl;
		}

		std::cout << "------------------------------" << std::endl;

		this->chain = chain;
		this->size = (unsigned int) chain.size();

	}

	void setSource(SoundSource& src) {
		this->src = &src;
	}

//	/** get amplitude of the source for the given time */
//	AudioData get(Time t) {
//		return filter(src->get(t));
//	}

	void process(Amplitude** inputs, Amplitude** outputs) override {

		vu.reset();
		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
			filter(inputs[0][i],inputs[1][i],  outputs[0][i],outputs[1][i]);
			vu.add(outputs[0][i]);
		}

	}

	void filter(Amplitude iLeft, Amplitude iRight, Amplitude& oLeft, Amplitude& oRight) {

		AudioData ad(iLeft, iRight);

		// original audio data
		AudioData ret = ad * dryWet;

		//ad = bq.filter(ad);

		// append to delay buffer
		//delay.filter(ad);
		delay.appendOnly(ad);

		// attach all reverb entries
		for (ReverbEntry re : chain) {
			//ret += delay.peekPercent(re.percent + 0.001 * pseudoRand()) * re.volume;		// better sound?
			ret += delay.peekPercent(re.percent) * re.volume;
		}

		// done
		oLeft = ret.left;
		oRight = ret.right;

	}

	unsigned int getNumParameters() const override {
		return 1;
	}

	Volume getVU() override {
		return vu.getMax();
	}

private:

	SoundSource* src;

	std::vector<ReverbEntry> chain;
	int size;

	/** the level between dry and wet [0-1] */
	Volume dryWet;

	/** the delay buffer to use */
	Delay delay;

	//BiquadFilter bq;

	/** vu calculation */
	VUCalc vu;

};



#endif /* REVERB_H_ */
