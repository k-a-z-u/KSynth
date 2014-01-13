/*
 * PatternSequencer.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kazu
 */

#ifndef PATTERNSEQUENCER_H_
#define PATTERNSEQUENCER_H_

#include "../notes/NoteDevice.h"
#include <mutex>
#include <vector>
#include "../SequencerListener.h"

#define PS_NUM_PATTERN 16

/** one note within one pattern */
struct PatternSequencerPatternEntry {

	/** the note to play */
	Note note;

	/** the volume use for playing the note */
	Volume volume;

	/** is this entry used at all? */
	bool used;

	/** empty ctor */
	PatternSequencerPatternEntry() :
		note(0), volume(0), used(false) {;}

	/** ctor */
	PatternSequencerPatternEntry(Note note, Volume volume) :
		note(note), volume(volume), used(true) {;}

};

/** one pattern for the sequencer */
class PatternSequencerPattern {

public:

	/** get the number of active bars */
	unsigned int getNumBars() const {return numBars;}

	/** get the granularity to use for each bar */
	unsigned int getGranularity() const {return granularity;}

	/** get the number of ticks (bars*granularity) */
	unsigned int getTicks() const {return numBars * granularity;}

	/** set a note within the pattern */
	void bindNote(unsigned int index, Note note, Volume volume) {
		entries[index] = PatternSequencerPatternEntry(note, volume);
	}

	/** remove the note behind the given index */
	void unbindNote(unsigned int index) {
		entries[index].used = false;
	}

	/** get all notes within this pattern */
	const std::vector<PatternSequencerPatternEntry>& getEntries() {
		return entries;
	}


protected:

	friend class PatternSequencer;

	/** empty ctor */
	PatternSequencerPattern() :
			numBars(4), granularity(4) {resize();}

	/** ctor */
	PatternSequencerPattern(unsigned int numBars, unsigned int granularity) :
		numBars(numBars), granularity(granularity) {resize();}

	/** set the number of active bars to use */
	void setNumBars(unsigned int numBars) {
		this->numBars = numBars;
		resize();
	}

	/** set the granularity to use for each bar (4th, 8th, ..) */
	void setGranulartity(unsigned int granularity) {
		this->granularity = granularity;
		resize();
	}

	/** resize to numBars and granularity */
	void resize() {
		mutex.lock();
		entries.resize(granularity * numBars);
		mutex.unlock();
		// TODO move entries when granularity changes
	}

	/** sequencer switches to new pattern. disable pending note */
	void disable() {

	}

private:

	/** all notes within this pattern */
	std::vector<PatternSequencerPatternEntry> entries;

	/** the number of bars to use (bar = takt) */
	unsigned int numBars;

	/** the note granularity to use for each bar. e.g. 4th, 8th, ... */
	unsigned int granularity;

	std::mutex mutex;


};

/**
 * simple pattern-based sequencer to create
 * note-patterns for note-devices
 */
class PatternSequencer : public SequencerBeatListener {

public:

	/** ctor */
	PatternSequencer() : lastBeat(0), currentPatternIdx(0), _currentPatternIdx(0), dev(nullptr) {

	}

	/** called for every new beat */
	void onBeat(Beat beat, Time time) override {

		(void) time;

		// (copy of) the current pattern
		//PatternSequencerPattern pat = pattern[_currentPattern];

		int curGranularity = pattern[_currentPatternIdx].granularity;

		// convert beat from 128th to patterns granularity
		beat /= (128 / curGranularity);

		// new beat? -> next pattern step
		if (beat != lastBeat) {

			lastBeat = beat;

			// switch to next pattern at the end of a bar
			if (beat % curGranularity == 0) {
				if (_currentPatternIdx != currentPatternIdx) {
					//pat.disable();
					_currentPatternIdx = currentPatternIdx;
					dev->stopNote(lastNote);
				}
			}

			PatternSequencerPattern& pat = pattern[_currentPatternIdx];
			pat.mutex.lock();

			// get last and current beat
			unsigned int nextIdx = (beat+0) % (unsigned int) pat.entries.size();
			unsigned int lastIdx = (beat-1) % (unsigned int) pat.entries.size();

			// stop last note and start next note
			if (pat.entries[lastIdx].used)	{
				dev->stopNote(lastNote);
				//dev->stopNote(pat.entries[lastIdx].note, time);
			}

			if (pat.entries[nextIdx].used)	{
				Note note = pat.entries[nextIdx].note;
				dev->startNote(note, pat.entries[nextIdx].volume);
				lastNote = note;
			}

			pat.mutex.unlock();

		}
	}



	/** the note-device target */
	void setTarget(NoteDevice* dev) {
		this->dev = dev;
	}

	/** set the number of acitve bars to use */
	void setNumBars(unsigned int numBars) {
		getCurrentPattern().setNumBars(numBars);
//		for (unsigned int i = 0; i < PS_NUM_PATTERN; ++i) {
//			pattern[i].setNumBars(numBars);
//		}
	}

	/** set the granularity to use for each bar (4th, 8th, ..) */
	void setGranulartity(unsigned int granularity) {
		getCurrentPattern().setGranulartity(granularity);
//		for (unsigned int i = 0; i < PS_NUM_PATTERN; ++i) {
//			pattern[i].setGranulartity(granularity);
//		}
	}

	/** get the i-th pattern */
	PatternSequencerPattern& getPattern(unsigned int idx) {
		return pattern[idx];
	}

	/** set the index for the current pattern to use */
	void setCurrentPattern(unsigned int idx) {
		this->currentPatternIdx = idx;
	}

	/** get the index of the currently active pattern */
	unsigned int getCurrentPatternIndex() {
		return this->currentPatternIdx;
	}

	/** get the currently active pattern */
	PatternSequencerPattern& getCurrentPattern() {
		return this->pattern[this->currentPatternIdx];
	}

private:

	/** the current beat */
	unsigned int lastBeat;

	/** all available patterns */
	PatternSequencerPattern pattern[PS_NUM_PATTERN];

	/** the current pattern */
	unsigned int currentPatternIdx;

	/** internal pointer to current pattern */
	unsigned int _currentPatternIdx;


	/** the device to send the notes to */
	NoteDevice* dev;


	/** the last played note */
	Note lastNote;

};



#endif /* PATTERNSEQUENCER_H_ */
