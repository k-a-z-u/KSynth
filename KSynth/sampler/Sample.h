/*
 * Sample.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef SAMPLE_H_
#define SAMPLE_H_

#include <KLib/fs/File.h>
#include "../misc/DataTypes.h"
#include <vector>
#include <string>

/**
 * represents a PCM audio sample
 */
class Sample {


public:

	/** ctor for an empty sample */
	Sample() : looping(false), sRate(0) {;}

	/** create sample from float [-1;+1] amplitude vector */
	Sample(std::vector<Amplitude> pcm, SampleRate rate, Channel channels) :
		looping(false), sRate(rate), channels(channels) {

		// TODO: add multiple channels
		this->pcm = pcm;

	}

	/** perform sample-rate conversion to the given value */
	void convertTo(SampleRate sRate) {
		convert(&pcm, this->sRate, sRate);
		this->sRate = sRate;
	}

	/** get the sample's length in milliseconds */
	unsigned int getLengthMilliSeconds() {
		return (unsigned int) pcm.size() * 1000 / this->sRate;
	}

	/** get the amount of used memory (in bytes) */
	unsigned int getMemoryUsage() {
		return (unsigned int) pcm.size();
	}

	/** get amplitude at the given time */
	AudioData get(SampleFrame frm) const {
		//unsigned int idxL = (unsigned int) (sRate * t);
		//unsigned int idxR = (unsigned int) (sRate * t * 1.002);
		//if (looping) {idxR %= pcm.size();} else if (idxR >= pcm.size()) {return 0.0;}
		//if (looping) {idxL %= pcm.size();} else if (idxL >= pcm.size()) {return 0.0;}
		//return (pcm[idxR], pcm[idxL]);
		if (looping) {
			frm %= (unsigned int) pcm.size();
		} else {
			if (frm >= pcm.size()) {return 0.0;}
		}
		return pcm[frm];

	}

	/** get the length in seconds */
	double getLength() {
		return 0;
	}

	/** get the used sample rate */
	SampleRate getSampleRate() const {return sRate;}

	/** get the number of channels */
	Channel getNumChannels() const {return channels;}

	/** get the sample's name (if any) */
	const std::string& getName() const {return name;}

	/** set the sample's name */
	void setName(const std::string& name) {this->name = name;}

	/** get the number of frames within this sample */
	SampleFrame getNumFrames() const {return SampleFrame(pcm.size());}

	/** get PCM data for the given channel */
	std::vector<Amplitude>* getAudioData(unsigned int channel) {
		// TODO: implement: more than one channel
		return &pcm;
	}

private:


	void convert(std::vector<Amplitude>* vec, SampleRate oldsr, SampleRate newsr) {

		// create a new vector for the converted PCM data
		SampleFrame samples = (unsigned int) (vec->size() * newsr / oldsr);
		std::vector<Amplitude> nvec;
		nvec.resize(samples);

		// convert using interpolation
		for (SampleFrame newSample = 0; newSample < samples; ++newSample) {

			// index within the old PCM data as float (for interpolation)
			float oldSample = (float) newSample * (float) oldsr / (float) newsr;

			SampleFrame oldSample1 = (unsigned int) std::floor(oldSample);
			SampleFrame oldSample2 = (unsigned int) std::ceil(oldSample);
			float blend1 = 1.0f - (oldSample - float(oldSample1));
			float blend2 = 1.0f - blend1;

			// end of vector?
			if (oldSample2 >= vec->size()) {break;}

			nvec[newSample] = (vec->at(oldSample1) * blend1) + (vec->at(oldSample2) * blend2);

		}

		// overwrite old vector
		*vec = nvec;

	}

	/** repeat this sample? */
	bool looping;

//	/** the length of the sample (in seconds) */
//	double length;

	/** the sample-rate in Hz */
	SampleRate sRate;

	/** the number of channels */
	Channel channels;

	/** the audio data. currently only mono! */
	std::vector<Amplitude> pcm;

	/** set a name for this pattern */
	std::string name;

};

#endif /* SAMPLE_H_ */
