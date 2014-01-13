/*
 * ADSR.h
 *
 *  Created on: Oct 23, 2013
 *      Author: kazu
 */

#ifndef ADSR_H_
#define ADSR_H_

#include "../misc/DataTypes.h"
#include "HuellFunction.h"


#include <iostream>

struct ADSRval {

	/** the time for attack (in ms) */
	int attack;

	/** the time for decay (in ms) */
	int decay;

	/** the volume to sustain */
	Volume sustian;

	/** the time for the release */
	int release;

	/** ctor */
	ADSRval(int a, int d, Volume s, int r) : attack(a), decay(d), sustian(s), release(r) {;}

};

class ADSR : public HuellFunction {

public:

	Amplitude get(const NoteDescription& nd, Time age) override {

		(void) nd;
		(void) age;
		return Amplitude(0);

//		if (nd.stop == 0) {

//			double ms = 1000*(age/48000.0f);
//			if (ms < adsr.attack) {return ms / adsr.attack;}
//			ms -= adsr.attack;
//			if (ms < adsr.decay) {return 1 - (ms / adsr.decay) * (1 - adsr.sustian) ;}
//			return adsr.sustian;
//		} else {
//			double ms = 1000*(age-nd.stop)/48000.0f;
//			if (ms < adsr.release) {return adsr.sustian - ms/adsr.release*adsr.sustian;}
//			return 0;
//		}

	}

//	bool done(const NoteDescription& nd, Time t) {
//		return (t-nd.stop > adsr.release/1000.0);
//	}

//	void setAttack(int a)		{this->adsr.attack = a;}
//	void setDecay(int d)		{this->adsr.decay = d;}
//	void setSustain(Volume s)	{this->adsr.sustian = s;}
//	void setRelease(int r)		{this->adsr.release = r;}

//	void setADSR(int a, int d, Volume s, int r) {
//		this->adsr.attack = a;
//		this->adsr.decay = d;
//		this->adsr.sustian = s;
//		this->adsr.release = r;
//	}

	//USE AN ARRAY WITH PRECOMPUTED VALUES FOR FASTER LOOKUPS ???

private:

	ADSRval adsr = ADSRval(2, 10, 0.75, 100);

};


#endif /* ADSR_H_ */
