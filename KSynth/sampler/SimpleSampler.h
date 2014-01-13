/*
 * SimpleSampler.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef SIMPLESAMPLER_H_
#define SIMPLESAMPLER_H_

#include "../notes/NoteDevice.h"
#include "../misc/SoundSource.h"
#include <unordered_map>
#include "Sample.h"



struct SimpleSamplerNote {

	/** the note to play */
	Note note;

	/** the time when the note was started */
	SampleFrame start;

	/** the time when the note was stopped. only some elements use this value */
	Time stop;

	/** the volume to use for playing */
	Volume vol;

	/** the sample behind the note */
	const Sample* sample;

	Time age;

	/** ctor */
	SimpleSamplerNote(Note note, SampleFrame start, Volume vol, const Sample* sample) :
		note(note), start(start), vol(vol), sample(sample), age(0) {;}

};


/**
 * play audio samples
 */
class SimpleSampler : public SoundSource, public NoteDevice {

public:

	/** ctor */
	SimpleSampler() : SoundSource(2), volume(0.75) {;}

	void startNote(Note note, Time start, Volume vol) override {

		// get the sample for this note, (if registered!)
		const auto& it = samples.find(note);
		if (it == samples.end()) {return;}

		// found a sample -> add
		nv.add(start*getSampleRate(), SimpleSamplerNote(note, start*getSampleRate(), vol, &it->second));

	}

	void stopNote(Note note, Time stop) override {
		nv.remove(note);
		//nv.cleanup(stop);
	}


	void process(Amplitude** inputs, Amplitude** outputs) override {

		static SampleFrame frm = 0;

		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {

			// process all currently active samples
			for (const SimpleSamplerNote& nd : nv.getEntries()) {
				AudioData ad = nd.sample->get(frm - nd.start) * nd.vol;
				outputs[0][i] += ad.left;
				outputs[1][i] += ad.right;
			}

			++frm;

			// volume
			outputs[0][i] *= volume;
			outputs[1][i] *= volume;

		}

	}

	/** add a sample for the given note */
	void addSample(Note n, const Sample& s) {
		samples[n] = s;
	}

	/** set the main volume */
	void setVolume(Volume v) {
		this->volume = v;
	}

private:

	/** all notes to play */
	NoteVector<SimpleSamplerNote> nv;

	/** sample for a specific note */
	std::unordered_map<Note, Sample> samples;

	/** the main volume */
	Volume volume;

};


#endif /* SIMPLESAMPLER_H_ */
