/*
 * SoundSinkNull.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kazu
 */

#ifndef SOUNDSINKNULL_H_
#define SOUNDSINKNULL_H_


/**
 * dummy sound sink
 */
class SoundSinkNull : public SoundSink {

public:

	/** ctor */
	SoundSinkNull() {;}

	/** dtor */
	~SoundSinkNull() {;}

	void push(AudioData a) override {
		(void) a;
	}

	void finalize() override {

	}

private:

};


#endif /* SOUNDSINKNULL_H_ */
