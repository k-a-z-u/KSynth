/*
 * SimpleDrumComputer.h
 *
 *  Created on: Dec 28, 2013
 *      Author: kazu
 */

#ifndef SIMPLEDRUMCOMPUTER_H_
#define SIMPLEDRUMCOMPUTER_H_

#include "../misc/SoundSource.h"
#include "../misc/Units.h"

#include "Sample.h"
#include "SampleFactory.h"
#include "../SequencerListener.h"
#include "../misc/VUCalc.h"
#include "../midi/MidiEvent.h"
#include <KLib/string/String.h>

/** the number of sample banks */
#define SDC_BANKS				8u

/** the number of different patterns */
#define SDC_PATTERNS			16u

/** the maximum lenght for each pattern (in 4th notes) */
#define SDC_PATTERN_MAX_LEN		32u

/** the minimum length for each pattern (in 4th notes) */
#define SDC_PATTERN_MIN_LEN		4u

/** the minimum pattern base (e.g. 1th notes) */
#define SDC_PATTERN_MIN_BASE	1u

/** the maximum pattern base (e.g. 8th notes) */
#define SDC_PATTERN_MAX_BASE	8u



struct SimpleDrumComputerSample {

	/** the sample to play */
	Sample s;

	/** the gain to applay to this sample */
	Volume gain;

	/** start playing the sample at the given offset (in percent) */
	float startOffset;

	/** ctor */
	SimpleDrumComputerSample() : gain(1.0f), startOffset(0.0f) {;}

};


/** entry for one beat within the drum computer patterns */
struct SimpleDrumComputerBankPatternEntry {

	/** ctor */
	SimpleDrumComputerBankPatternEntry() : vol(0.0f) {;}

	/** is this entry set? (volume > 0.0) */
	bool isSet() const {
		return vol > 0.0f;
	}

	/** unset this entry */
	void unset() {
		vol = 0.0f;
	}


	/** the volume for playing this entry. 0.0 = disabled */
	Volume vol;

};



/**
 * all selected beats for a specific bank
 * e.g.: all beats for the bass-drum
 */
class SimpleDrumComputerBankPattern {

public:

	/** ctor */
	SimpleDrumComputerBankPattern() {
		;
	}

	/** unset all entries */
	void clear() {
		for (unsigned int i = 0; i < SDC_PATTERN_MAX_LEN; ++i) {entries[i].unset();}
	}

	/** set the volume for playing the idx-th entry (0.0 = disable) */
	void set(unsigned int idx, Volume vol) {
		if (idx >= SDC_PATTERN_MAX_LEN) {return;}
		entries[idx].vol = vol;
	}

	/** get the volume for the idx-th entry */
	Volume get(unsigned int idx) const {
		if (idx >= SDC_PATTERN_MAX_LEN) {return 0.0f;}
		return entries[idx].vol;
	}

	/** is the idx-th entry set? */
	bool isSet(unsigned int idx) {
		if (idx >= SDC_PATTERN_MAX_LEN) {return false;}
		return entries[idx].isSet();
	}


private:

	/** all beats */
	SimpleDrumComputerBankPatternEntry entries[SDC_PATTERN_MAX_LEN];

};



/** the drum computer provides 16 distinct patterns each covering all 8 banks */
class SimpleDrumComputerPattern {

public:

	/** ctor */
	SimpleDrumComputerPattern() : length(SDC_PATTERN_MAX_LEN), base(4) {;}


	/** unset all entries */
	void clear() {
		for (unsigned int i = 0; i < SDC_BANKS; ++i) {banks[i].clear();}
	}

	/** get the i-th bank */
	SimpleDrumComputerBankPattern* getBank(unsigned int bank) const {
		if (bank >= SDC_BANKS) {return nullptr;}
		return (SimpleDrumComputerBankPattern*) &banks[bank];
	}

	/** get the pattern's length */
	unsigned int getLength() const {
		return length;
	}

	/** set the pattern's length */
	void setLength(unsigned int len) {
		if (len < SDC_PATTERN_MIN_LEN) {return;}
		if (len > SDC_PATTERN_MAX_LEN) {return;}
		this->length = len;
	}

	/** get the pattern's base (e.g. 4th notes) */
	unsigned int getBase() const {
		return base;
	}

	/** set the pattern's base (e.g. 4th notes) */
	void setBase(unsigned int base) {
		this->base = base;
	}

private:

	/** 4/4 beat -> 16 entries */
	SimpleDrumComputerBankPattern banks[SDC_BANKS];

	/** the pattern's length */
	unsigned int length;

	/** the speed base (e.g. 4th notes) */
	unsigned int base;

};



struct SimpleDrumComputerNote {
	Sample* sample;
	SampleFrame pos;
	Volume vol;
	SimpleDrumComputerNote(Sample* sample, SampleFrame pos, Volume vol) :
		sample(sample), pos(pos), vol(vol) {;}
};


/** all available parameters for the SimpleDrumComputer */
enum class SimpleDrumComputerParameter {

	PATTERN_IDX,
	PATTERN_LENGTH,
	PATTERN_BASE,

	MAIN_VOLUME,

	BANK1_VOLUME,
	BANK2_VOLUME,
	BANK3_VOLUME,
	BANK4_VOLUME,
	BANK5_VOLUME,
	BANK6_VOLUME,
	BANK7_VOLUME,
	BANK8_VOLUME,

	BANK1_START_OFFSET,
	BANK2_START_OFFSET,
	BANK3_START_OFFSET,
	BANK4_START_OFFSET,
	BANK5_START_OFFSET,
	BANK6_START_OFFSET,
	BANK7_START_OFFSET,
	BANK8_START_OFFSET,

	BANK1_FILE,
	BANK2_FILE,
	BANK3_FILE,
	BANK4_FILE,
	BANK5_FILE,
	BANK6_FILE,
	BANK7_FILE,
	BANK8_FILE,

	_END,

};

/**
 * a simple drum-computer with several patterns
 */
class SimpleDrumComputer : public SoundSource, public SequencerBeatListener {

public:

	/** ctor */
	SimpleDrumComputer() : SoundSource(2), masterGain(1.0f) {
		;
	}

	~SimpleDrumComputer() {
		;
	}

	/** get the amount of used memory (in bytes) */
	unsigned int getMemoryUsage() {
		unsigned int ret = 0;
		for (SimpleDrumComputerSample& s : samples) {
			ret += s.s.getMemoryUsage();
		}
		return ret;
	}

	/** take the given input (if any!) and provide the corresponding output */
	void process(Amplitude** input, Amplitude** output) {

		(void) input;

		// process all (active) notes (samples)
		for (unsigned int j = 0; j < notes.size(); ++j) {

			SimpleDrumComputerNote& n = notes[j];

			for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
				output[0][i] += n.sample->get(n.pos).left * n.vol;
				output[1][i] += n.sample->get(n.pos).right * n.vol;
				++n.pos;
			}
			if (n.pos > n.sample->getNumFrames()) {notes.erase(notes.begin()+j); --j;}
		}
		vu.get(output[0], getSamplesPerProcess());
	}


	void onBeat(Beat beat, Time time) override {

		(void) time;

		// convert beat from 128-th to x-th
		unsigned int _beat = beat * pattern[current.patternIdx].getBase() / 128;
		if (current.beat == _beat) {return;}
		current.beat = _beat;

		// convert beat to 0 - pattern's length
		current.patternBeat = current.beat % pattern[current.patternIdx].getLength();

		// check currently active pattern for each bank and activate their samples
		for (unsigned int b = 0; b < SDC_BANKS; ++b) {
			if (pattern[current.patternIdx].getBank(b)->isSet(current.patternBeat)) {
				Volume v = pattern[current.patternIdx].getBank(b)->get(current.patternBeat);
				addBeatFor(b, v);
			}
		}

	}

	unsigned int getNumParameters() const override {
		return (int) SimpleDrumComputerParameter::_END;
	}

	void setParmeter(SimpleDrumComputerParameter p, ParamValue v) {
		switch(p) {
			case SimpleDrumComputerParameter::MAIN_VOLUME:			masterGain = Units::valueToGain(v); break;

			case SimpleDrumComputerParameter::PATTERN_IDX:			setPattern( v.asInt(0, SDC_PATTERNS-1) ); break;
			case SimpleDrumComputerParameter::PATTERN_LENGTH:		getCurrentPattern()->setLength( v.asUInt(SDC_PATTERN_MIN_LEN, SDC_PATTERN_MAX_LEN) ); break;
			case SimpleDrumComputerParameter::PATTERN_BASE:			getCurrentPattern()->setBase( v.asUInt(SDC_PATTERN_MIN_BASE, SDC_PATTERN_MAX_BASE) ); break;

			case SimpleDrumComputerParameter::BANK1_VOLUME:			samples[0].gain = Units::valueToGain(v); break;
			case SimpleDrumComputerParameter::BANK2_VOLUME:			samples[1].gain = Units::valueToGain(v); break;
			case SimpleDrumComputerParameter::BANK3_VOLUME:			samples[2].gain = Units::valueToGain(v); break;
			case SimpleDrumComputerParameter::BANK4_VOLUME:			samples[3].gain = Units::valueToGain(v); break;
			case SimpleDrumComputerParameter::BANK5_VOLUME:			samples[4].gain = Units::valueToGain(v); break;
			case SimpleDrumComputerParameter::BANK6_VOLUME:			samples[5].gain = Units::valueToGain(v); break;
			case SimpleDrumComputerParameter::BANK7_VOLUME:			samples[6].gain = Units::valueToGain(v); break;
			case SimpleDrumComputerParameter::BANK8_VOLUME:			samples[7].gain = Units::valueToGain(v); break;

			case SimpleDrumComputerParameter::BANK1_START_OFFSET:	samples[0].startOffset = v; break;
			case SimpleDrumComputerParameter::BANK2_START_OFFSET:	samples[1].startOffset = v; break;
			case SimpleDrumComputerParameter::BANK3_START_OFFSET:	samples[2].startOffset = v; break;
			case SimpleDrumComputerParameter::BANK4_START_OFFSET:	samples[3].startOffset = v; break;
			case SimpleDrumComputerParameter::BANK5_START_OFFSET:	samples[4].startOffset = v; break;
			case SimpleDrumComputerParameter::BANK6_START_OFFSET:	samples[5].startOffset = v; break;
			case SimpleDrumComputerParameter::BANK7_START_OFFSET:	samples[6].startOffset = v; break;
			case SimpleDrumComputerParameter::BANK8_START_OFFSET:	samples[7].startOffset = v; break;

			default:												break;
		}
	}

	ParamValue getParameter(SimpleDrumComputerParameter p) const {
		switch(p) {
			case SimpleDrumComputerParameter::MAIN_VOLUME:			return Units::gainToValue(masterGain);

			case SimpleDrumComputerParameter::PATTERN_IDX:			return ParamValue(0u, SDC_PATTERNS-1, current.patternIdx);
			case SimpleDrumComputerParameter::PATTERN_LENGTH:		return ParamValue(SDC_PATTERN_MIN_LEN, SDC_PATTERN_MAX_LEN, getCurrentPatternLength());
			case SimpleDrumComputerParameter::PATTERN_BASE:			return ParamValue(SDC_PATTERN_MIN_BASE, SDC_PATTERN_MAX_BASE, getCurrentPatternBase());
			case SimpleDrumComputerParameter::BANK1_VOLUME:			return Units::gainToValue(samples[0].gain);
			case SimpleDrumComputerParameter::BANK2_VOLUME:			return Units::gainToValue(samples[1].gain);
			case SimpleDrumComputerParameter::BANK3_VOLUME:			return Units::gainToValue(samples[2].gain);
			case SimpleDrumComputerParameter::BANK4_VOLUME:			return Units::gainToValue(samples[3].gain);
			case SimpleDrumComputerParameter::BANK5_VOLUME:			return Units::gainToValue(samples[4].gain);
			case SimpleDrumComputerParameter::BANK6_VOLUME:			return Units::gainToValue(samples[5].gain);
			case SimpleDrumComputerParameter::BANK7_VOLUME:			return Units::gainToValue(samples[6].gain);
			case SimpleDrumComputerParameter::BANK8_VOLUME:			return Units::gainToValue(samples[7].gain);

			case SimpleDrumComputerParameter::BANK1_START_OFFSET:	return samples[0].startOffset;
			case SimpleDrumComputerParameter::BANK2_START_OFFSET:	return samples[1].startOffset;
			case SimpleDrumComputerParameter::BANK3_START_OFFSET:	return samples[2].startOffset;
			case SimpleDrumComputerParameter::BANK4_START_OFFSET:	return samples[3].startOffset;
			case SimpleDrumComputerParameter::BANK5_START_OFFSET:	return samples[4].startOffset;
			case SimpleDrumComputerParameter::BANK6_START_OFFSET:	return samples[5].startOffset;
			case SimpleDrumComputerParameter::BANK7_START_OFFSET:	return samples[6].startOffset;
			case SimpleDrumComputerParameter::BANK8_START_OFFSET:	return samples[7].startOffset;

			default:												return ParamValue(0.0f);
		}
	}

	void setParameter(Param p, ParamValue v) override {
		setParmeter((SimpleDrumComputerParameter) p, v);
	}

	ParamValue getParameter(Param p) const override {
		return getParameter((SimpleDrumComputerParameter) p);
	}

	void setParameterName(SimpleDrumComputerParameter p, const std::string &name) {
		switch(p) {
			case SimpleDrumComputerParameter::BANK1_FILE:	loadSample(name, 0); break;
			case SimpleDrumComputerParameter::BANK2_FILE:	loadSample(name, 1); break;
			case SimpleDrumComputerParameter::BANK3_FILE:	loadSample(name, 2); break;
			case SimpleDrumComputerParameter::BANK4_FILE:	loadSample(name, 3); break;
			case SimpleDrumComputerParameter::BANK5_FILE:	loadSample(name, 4); break;
			case SimpleDrumComputerParameter::BANK6_FILE:	loadSample(name, 5); break;
			case SimpleDrumComputerParameter::BANK7_FILE:	loadSample(name, 6); break;
			case SimpleDrumComputerParameter::BANK8_FILE:	loadSample(name, 7); break;
			default:										break;
		}
	}

	void setParameterName(Param p, const std::string &name) override {
		setParameterName((SimpleDrumComputerParameter)p, name);
	}

	const std::string getParameterName(SimpleDrumComputerParameter p) const {
		switch(p) {
			case SimpleDrumComputerParameter::BANK1_FILE:	return samples[0].s.getName();
			case SimpleDrumComputerParameter::BANK2_FILE:	return samples[1].s.getName();
			case SimpleDrumComputerParameter::BANK3_FILE:	return samples[2].s.getName();
			case SimpleDrumComputerParameter::BANK4_FILE:	return samples[3].s.getName();
			case SimpleDrumComputerParameter::BANK5_FILE:	return samples[4].s.getName();
			case SimpleDrumComputerParameter::BANK6_FILE:	return samples[5].s.getName();
			case SimpleDrumComputerParameter::BANK7_FILE:	return samples[6].s.getName();
			case SimpleDrumComputerParameter::BANK8_FILE:	return samples[7].s.getName();
			default:										return "";
		}
	}

	const std::string getParameterName(Param p) const override {
		return getParameterName( (SimpleDrumComputerParameter) p );
	}


#include <sstream>
	virtual std::string getChunkData() const override {

		std::stringstream ss;

		// export each pattern
		for (unsigned int i = 0; i < SDC_PATTERNS; ++i) {

			ss << i << ' ';
			ss << pattern[i].getLength() << ' ';
			ss << pattern[i].getBase();

			for (unsigned int j = 0; j < SDC_BANKS; ++j) {
				ss << '\n' << j;

				for (unsigned int k = 0; k < pattern[i].getLength(); ++k) {
					ss << ' ' << pattern[i].getBank(j)->get(k);
				}

			}

			ss << "\n";

		}

		return ss.str();

	}

	virtual void setChunkData(const std::string& data) override {

		// remove all linebreaks
		std::string tmp = data;
		K::String::replace(tmp, "\n", " ");
		std::stringstream ss(tmp);

		unsigned int len;
		unsigned int base;
		unsigned int pIdx = 0;
		unsigned int sIdx = 0;
		Volume vol;

		for (unsigned int pi = 0; pi < SDC_PATTERNS; ++pi) {

			ss >> pIdx; ss.ignore(1);
			ss >> len; ss.ignore(1);
			ss >> base; ss.ignore(1);
			pattern[pIdx].setLength(len);
			pattern[pIdx].setBase(base);

			if (pIdx != pi) {throw "error while reading";}

			for (unsigned int si = 0; si < SDC_BANKS; ++si) {
				ss >> sIdx; ss.ignore(1);
				if (sIdx != si) {throw "error while reading";}

				for (unsigned int j = 0; j < len; ++j) {
					ss >> vol; ss.ignore(1);
					pattern[pi].getBank(si)->set(j, vol);
				}

			}

		}

		setPattern(0);

	}



	std::string getProductString() const override {
		return "SimpleDrumComputer";
	}

	bool getOutputProperties(unsigned int idx, PinProperties* properties) override {
		switch(idx) {
			case 0:	properties->name = "output (left)"; return true;
			case 1:	properties->name = "output (right)"; return true;
		}
		return false;
	}

	virtual Volume getVU() override {
		return vu.getMax();
	}

	virtual void processEvents(const Events* evts) override {
		for (unsigned int i = 0; i < evts->numEvents; ++i) {
			Event e = evts->events[i];
			if (e.type != EventType::MIDI) {continue;}
			MidiEvent* me = (MidiEvent*) e.data;
			processMidiEvent(me);
		}
	}


	/** set the currently active pattern for all 8 banks */
	void setPattern(unsigned int idx) {
		if (idx >= SDC_PATTERNS) {return;}
		current.patternIdx = idx;
	}

	/** get the pattern for all 8 banks */
	SimpleDrumComputerPattern* getPattern(unsigned int idx) {
		if (idx >= SDC_PATTERNS) {return nullptr;}
		return &pattern[idx];
	}

	/** get the currently active pattern for all 8 banks */
	SimpleDrumComputerPattern* getCurrentPattern() {
		return &pattern[current.patternIdx];
	}

	/** get the length of the current pattern */
	unsigned int getCurrentPatternLength() const {
		return pattern[current.patternIdx].getLength();
	}

	/** get the base (e.g. 4th notes) of the current pattern */
	unsigned int getCurrentPatternBase() const {
		return pattern[current.patternIdx].getBase();
	}


	/** load sample for the given bank */
	void loadSample(const std::string& file, unsigned int bank) {
		if (bank >= SDC_BANKS) {return;}
		if (file.empty()) {
			samples[bank].s = Sample();
			samples[bank].s.setName("");
		} else {
			K::File f(file);
			samples[bank].s = SampleFactory::load(f, getSampleRate());
			samples[bank].s.setName(f.getAbsolutePath());
		}
	}

	/** get the sample behind the given bank */
	const Sample* getSample(unsigned int bank) {
		if (bank >= SDC_BANKS) {return nullptr;}
		return &samples[bank].s;
	}

	/** get the current beat-number within the currently active pattern */
	Beat getCurrentBeat() {
		return current.patternBeat;
	}


protected:

	/**
	 * add a beat (play it) for the sample behind the given bank
	 * playing it using the provided volume
	 * @param bank the bank of the sample to play
	 * @param vol the volume for the playback
	 */
	void addBeatFor(unsigned int bank, Volume vol) {
		SampleFrame offset = (SampleFrame) (samples[bank].s.getNumFrames() * samples[bank].startOffset);
		notes.push_back(SimpleDrumComputerNote(&samples[bank].s, offset, samples[bank].gain * vol * masterGain));
	}

	/** process the given midi event */
	void processMidiEvent(MidiEvent* me) {
		if (me->getType() == MidiEventType::NOTE_ON) {
			MidiEventNoteOn* meon = (MidiEventNoteOn*) me;
			unsigned int nr = meon->getNote();
			if (nr >= SDC_BANKS) {return;}
			addBeatFor(nr, meon->getVolume());
		}
	}

	/** audio sample for each of the 8 banks */
	SimpleDrumComputerSample samples[SDC_BANKS];

	/** the drum computer provides 16 distinct patterns each covering all 8 banks */
	SimpleDrumComputerPattern pattern[SDC_PATTERNS];


	/** the current state */
	struct Current {

		/** the currently selected pattern */
		unsigned int patternIdx;

		/** the current beat */
		unsigned int beat;

		/** beat modulo pattern's size */
		unsigned int patternBeat;

		/** ctor */
		Current() : patternIdx(0), beat(0), patternBeat(0) {;}

	} current;



	/** all currently active notes */
	std::vector<SimpleDrumComputerNote> notes;

	/** VU calculation */
	VUCalc vu;

	/** the main volume of the drum computer */
	Volume masterGain;

};


#endif /* SIMPLEDRUMCOMPUTER_H_ */
