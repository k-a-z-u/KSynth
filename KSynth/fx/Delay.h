/*
 * Delay.h
 *
 *  Created on: Oct 31, 2013
 *      Author: kazu
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "../fx/RingBufferFixed.h"
#include "../misc/SoundProcessor.h"
#include "../misc/AudioFormat.h"
//#include <list>


/**
 * puts a delay to the output
 */
class Delay : public SoundProcessor {

public:

	/** ctor */
	Delay(SampleRate srate) :
		SoundProcessor(2,2), srate(srate), src(&soundSourceDummy), buffer(), bufSize(0) {
		;
	}

	/** get the latest value inserted into the delay-buffer */
	AudioData getLatest() {
		return buffer.getHead();
	}

	void process(Amplitude** inputs, Amplitude** outputs) override {

		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
			AudioData ad(*inputs[0]++, *inputs[1]++);
			AudioData ret = filter(ad);
			*outputs[0]++ = ret.left;
			*outputs[1]++ = ret.right;
		}

	}

	std::string getProductString() const override {
		return "Delay";
	}

	/** set the delay to use */
	void setDelay(Time t) {
		bufSize = (unsigned int) (t * Time(srate) + 1);
	}

	/** set the delay to use immediately without adjusting it over time */
	void setDelayImmediately(Time t) {
		setDelay(t);
		buffer.setUsed(bufSize);
	}

	/** filter the input data */
	AudioData filter(AudioData ad) {

		buffer.push(ad);

		// get the value to return
		AudioData ret = buffer.pop();

		// remove from the buffer
		//buffer.pop();

		// adapt to requested delay
		if		(buffer.bytesUsed() < bufSize)	{buffer.push(ad);}
		else if	(buffer.bytesUsed() > bufSize)	{buffer.pop();}

		// done
		return ret;

	}


	/** only append a new value without fetching the next result */
	void appendOnly(AudioData ad) {

		buffer.push(ad);

		// adapt to requested delay
		if		(buffer.bytesUsed() < bufSize)	{buffer.push(ad);}
		else if	(buffer.bytesUsed() > bufSize)	{buffer.pop();}

	}

	/** fetch the value at x percent */
	AudioData peekPercent(float percent) {
		return buffer.peekBehindHead( int( float(buffer.bytesUsed()) * percent) );
	}


	/** set the input source for the delay filter */
	void setSource(SoundSource* src) {
		if (src == nullptr)	{this->src = &soundSourceDummy;}
		else				{this->src = src;}
	}

	unsigned int getNumParameters() const override {
		return 1;
	}

private:

	/** the audio format of the output */
	SampleRate srate;

	/** the source to build the delay for */
	SoundSource* src;

	/** the history buffer */
	RingBufferFixed<AudioData, 65536> buffer;

	unsigned int bufSize;

};


#endif /* DELAY_H_ */
