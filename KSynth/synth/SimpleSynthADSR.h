/*
 * SimpleSynthADSR.h
 *
 *  Created on: Dec 8, 2013
 *      Author: kazu
 */

#ifndef SIMPLESYNTHADSR_H_
#define SIMPLESYNTHADSR_H_

/**
 * ADSR provider for the simple synth
 *
 * uses pre-calculated look-up tables
 *
 */
class SimpleSynthADSR {

public:

	/** get ADSR volume for a (de)activated note of the given age in sample-frames */
	Volume get(bool off, SampleFrame age) {
		if (!off) {
			if (age >= arrAD.len) {return params.sustain;}
			return arrAD.data[age];
		} else {
			if (age >= arrR.len) {return 0;}
			return arrR.data[age];
		}
	}

	void setSampleRate(SampleRate rate)	{this->params.rate = rate; recalc();}


	/** set the attack (in ms between [0, 100]) */
	void setAttack(unsigned int a)		{this->params.attack = a; recalc();}

	/** get the attack (in ms between [0, 100]) */
	unsigned int getAttack() const		{return this->params.attack;}


	/** set the decay (in ms between [0, 100]) */
	void setDecay(unsigned int d)		{this->params.decay = d; recalc();}

	/** get the decay (in ms between [0, 100]) */
	unsigned int getDecay() const		{return this->params.decay;}


	/** set the sustain volume (between [0,1]) */
	void setSustain(Volume s)			{this->params.sustain = s; recalc();}

	/** get the sustain volume (between [0,1]) */
	Volume getSustain() const			{return this->params.sustain;}


	/** set the release (in ms between [0,1000]) */
	void setRelease(unsigned int r)		{this->params.release = r; recalc();}

	/** get the release (in ms between [0,1000]) */
	unsigned int getRelease() const		{return this->params.release;}


	void setADSR(unsigned int a, unsigned int d, Volume s, unsigned int r, SampleRate rate) {
		this->params.attack = a;
		this->params.decay = d;
		this->params.sustain = s;
		this->params.release = r;
		this->params.rate = rate;
		recalc();
	}

private:

	/** recalculate array */
	void recalc() {

		// necessary frames for A, D and R
		unsigned int framesA = params.attack * params.rate / 1000;
		unsigned int framesD = params.decay * params.rate / 1000;
		unsigned int framesR = params.release * params.rate / 1000;

		arrAD.len = framesA + framesD;
		arrR.len = framesR;

		// calculate attack
		for (unsigned int i = 0; i < framesA; ++i) {
			arrAD.data[i] = (Volume) i /  (Volume) framesA;
		}

		// calculate decay
		for (unsigned int i = 0; i < framesD; ++i) {
			{arrAD.data[i+framesA] = Volume(1) - (Volume(i) / Volume(framesD)) * (Volume(1) - params.sustain);}
		}

		// calculate release
		for (unsigned int i = 0; i < framesR; ++i) {
			arrR.data[i] = params.sustain - ((Volume) i / (Volume) framesR) * params.sustain;
		}

	}


private:

	/** look-up table for A,D and R */
	struct Array {
		Volume data [48000];
		unsigned int len;
		Array() : len(0) {;}
	};

	/** LUT for attack and decay */
	Array arrAD;

	/** LUT for release */
	Array arrR;

	/** parameters */
	struct {
		unsigned int attack;
		unsigned int decay;
		Volume sustain;
		unsigned int release;
		SampleRate rate;
	} params;

};


#endif /* SIMPLESYNTHADSR_H_ */
