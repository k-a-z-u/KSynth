/*
 * SimpleSynth.h
 *
 *  Created on: Oct 23, 2013
 *      Author: kazu
 */

#ifndef SIMPLESYNTH_H_
#define SIMPLESYNTH_H_

//#include "../oscillator/Oscillator.h"
#include "../oscillator/SimpleOscillator2.h"

#include "../fx/BiquadFilter.h"
#include "../misc/VUCalc.h"
#include "../huell/ADSR.h"
#include "../notes/Note.h"
#include "../misc/DataTypes.h"
#include "../notes/NoteVector.h"
#include "../misc/SoundSource.h"
#include "../notes/NoteDevice.h"
#include <cassert>
//#include "../lib/misc/RingBuffer.h"
#include "SimpleSynthADSR.h"
#include "AGC.h"
#include <memory>

//#define USE_SSE
//#include "../misc/SSE.h"

struct SimpleSynthNoteDescription : public NoteDescription {

	/** current phase for OSC1 */
	double curPhase1;

	/** current phase for OSC2 */
	double curPhase2;

	/** filtering for each note (due to huell curves) */
	std::shared_ptr<BiquadFilter> bq;

	/** ctor */
	SimpleSynthNoteDescription(Note note, Volume vol) :
		NoteDescription(note, vol), curPhase1(0), curPhase2(0), bq(new BiquadFilter()) {
		;
	}



};

/** all configurable params for the SimpleSynth */
enum class SimpleSynthParams {

	OSC_HUELL_A,
	OSC_HUELL_D,
	OSC_HUELL_S,
	OSC_HUELL_R,

	FILTER_HUELL_A,
	FILTER_HUELL_D,
	FILTER_HUELL_S,
	FILTER_HUELL_R,

	OSC1_USE_KEYBOARD,
	OSC1_OCTAVE,
	OSC1_SEMI,
	OSC1_FINE,
	OSC1_MODE,
	OSC1_PHASE_OFFSET,

	OSC2_USE_KEYBOARD,
	OSC2_OCTAVE,
	OSC2_SEMI,
	OSC2_FINE,
	OSC2_MODE,
	OSC2_PHASE_OFFSET,

	OSC_MIX,

	LFO1_FREQUENCY,
	LFO1_MODE,
	LFO1_AMOUNT,

	FILTER_FREQUENCY,
	FILTER_RESONANCE,

	USE_RING_MODULATION,
	OUTPUT_VOLUME,

	_END,

};

class SimpleSynth;

//typedef void (*paramFunction)(double);
#include <algorithm>
typedef std::function<void(const SimpleSynth&, double)> paramFunction;


/**
 * a simple synthesizer
 */
class SimpleSynth : public SoundSource, public NoteDevice {

public:

	/** ctor */
	SimpleSynth() :
		SoundSource(2), useRingMod(false), mix(0.5), agc(getSampleRate(), getSamplesPerProcess()) {

		osc[0].osc.setMode(SimpleOscillator2Mode::SQUARE_NO_ALIAS);
		osc[1].osc.setMode(SimpleOscillator2Mode::SQUARE_NO_ALIAS);

		adsr.setADSR(5, 5, 0.75, 5, getSampleRate());
		filter.adsr.setADSR(32, 0, 0.25, 0, getSampleRate());

		masterGain = 1.0f;

	}

	/** dtor */
	~SimpleSynth() {;}

	void setParameter(Param p, ParamValue v) override {
		SimpleSynthParams pp = (SimpleSynthParams) p;
		switch (pp) {

			case SimpleSynthParams::OSC_HUELL_A:			adsr.setAttack( int(v * 100) ); break;
			case SimpleSynthParams::OSC_HUELL_D:			adsr.setDecay( int(v * 100) ); break;
			case SimpleSynthParams::OSC_HUELL_S:			adsr.setSustain(v); break;
			case SimpleSynthParams::OSC_HUELL_R:			adsr.setRelease( int(v * 1000) ); break;

			case SimpleSynthParams::FILTER_HUELL_A:			filter.adsr.setAttack( int(v * 100) ); break;
			case SimpleSynthParams::FILTER_HUELL_D:			filter.adsr.setDecay( int(v * 100) ); break;
			case SimpleSynthParams::FILTER_HUELL_S:			filter.adsr.setSustain(v); break;
			case SimpleSynthParams::FILTER_HUELL_R:			filter.adsr.setRelease( int(v * 1000) ); break;

			case SimpleSynthParams::OSC1_MODE:				osc[0].osc.setMode( (SimpleOscillator2Mode)(v.asInt(0, (int)SimpleOscillator2Mode::_END-1)) ); break;
			case SimpleSynthParams::OSC2_MODE:				osc[1].osc.setMode( (SimpleOscillator2Mode)(v.asInt(0, (int)SimpleOscillator2Mode::_END-1)) ); break;

			case SimpleSynthParams::OSC1_OCTAVE:			osc[0].octave = v.asInt(0,8); break;
			case SimpleSynthParams::OSC2_OCTAVE:			osc[1].octave = v.asInt(0,8); break;

			case SimpleSynthParams::OSC1_SEMI:				osc[0].semi = v.asInt(0,11); break;
			case SimpleSynthParams::OSC2_SEMI:				osc[1].semi = v.asInt(0,11); break;

			case SimpleSynthParams::OSC1_FINE:				osc[0].fine = 1.0f + (v-0.5f)/100.0f; break;
			case SimpleSynthParams::OSC2_FINE:				osc[1].fine = 1.0f + (v-0.5f)/100.0f; break;

			case SimpleSynthParams::OSC1_USE_KEYBOARD:		osc[0].fromKeyboard = v.asBool(); break;
			case SimpleSynthParams::OSC2_USE_KEYBOARD:		osc[1].fromKeyboard = v.asBool(); break;

			case SimpleSynthParams::OSC1_PHASE_OFFSET:		osc[0].phaseOffset = v; break;
			case SimpleSynthParams::OSC2_PHASE_OFFSET:		osc[1].phaseOffset = v; break;

			case SimpleSynthParams::USE_RING_MODULATION:	useRingMod = v.asBool(); break;
			case SimpleSynthParams::OUTPUT_VOLUME:			masterGain = Units::valueToGain(v); break;

			case SimpleSynthParams::OSC_MIX:				mix = v; break;

			case SimpleSynthParams::LFO1_MODE:				lfo[0].osc.setMode((SimpleOscillator2Mode) v.asInt(0, (int) SimpleOscillator2Mode::_END-1)); break;
			case SimpleSynthParams::LFO1_FREQUENCY:			lfo[0].freq = v.asFloat(0.0f, 10.0f); break;
			case SimpleSynthParams::LFO1_AMOUNT:			lfo[0].amount = v; break;

			case SimpleSynthParams::FILTER_FREQUENCY:		filter.freq = v; break;
			case SimpleSynthParams::FILTER_RESONANCE:		filter.res = v;	break;

			default: break;

		}
	}

	ParamValue getParameter(Param p) const override {

		SimpleSynthParams pp = (SimpleSynthParams) p;
		switch (pp) {

			case SimpleSynthParams::OSC_HUELL_A:			return ParamValue(0,100, adsr.getAttack());
			case SimpleSynthParams::OSC_HUELL_D:			return ParamValue(0,100, adsr.getDecay());
			case SimpleSynthParams::OSC_HUELL_S:			return adsr.getSustain();
			case SimpleSynthParams::OSC_HUELL_R:			return ParamValue(0,1000, adsr.getRelease());

			case SimpleSynthParams::FILTER_HUELL_A:			return ParamValue(0,100, filter.adsr.getAttack());
			case SimpleSynthParams::FILTER_HUELL_D:			return ParamValue(0,100, filter.adsr.getDecay());
			case SimpleSynthParams::FILTER_HUELL_S:			return filter.adsr.getSustain();
			case SimpleSynthParams::FILTER_HUELL_R:			return ParamValue(0,1000, filter.adsr.getRelease());

			case SimpleSynthParams::OSC1_MODE:				return ParamValue(0, (int)SimpleOscillator2Mode::_END-1, (int)osc[0].osc.getMode());
			case SimpleSynthParams::OSC2_MODE:				return ParamValue(0, (int)SimpleOscillator2Mode::_END-1, (int)osc[1].osc.getMode());

			case SimpleSynthParams::OSC1_OCTAVE:			return ParamValue(0,8, osc[0].octave);
			case SimpleSynthParams::OSC2_OCTAVE:			return ParamValue(0,8, osc[1].octave);

			case SimpleSynthParams::OSC1_SEMI:				return ParamValue(0,11, osc[0].semi);
			case SimpleSynthParams::OSC2_SEMI:				return ParamValue(0,11, osc[1].semi);

			case SimpleSynthParams::OSC1_FINE:				return (float) ((osc[0].fine-1.0f)*100+0.5f); break;
			case SimpleSynthParams::OSC2_FINE:				return (float) ((osc[1].fine-1.0f)*100+0.5f); break;

			case SimpleSynthParams::OSC1_USE_KEYBOARD:		return ParamValue(osc[0].fromKeyboard);
			case SimpleSynthParams::OSC2_USE_KEYBOARD:		return ParamValue(osc[1].fromKeyboard);

			case SimpleSynthParams::OSC1_PHASE_OFFSET:		return osc[0].phaseOffset; break;
			case SimpleSynthParams::OSC2_PHASE_OFFSET:		return osc[1].phaseOffset; break;

			case SimpleSynthParams::USE_RING_MODULATION:	return ParamValue(useRingMod);
			case SimpleSynthParams::OUTPUT_VOLUME:			return Units::gainToValue(masterGain);

			case SimpleSynthParams::OSC_MIX:				return mix;

			case SimpleSynthParams::LFO1_MODE:				return ParamValue(0, (int)SimpleOscillator2Mode::_END-1, (int) lfo[0].osc.getMode());
			case SimpleSynthParams::LFO1_FREQUENCY:			return ParamValue(0.0f, 10.0f, lfo[0].freq);
			case SimpleSynthParams::LFO1_AMOUNT:			return lfo[0].amount;

			case SimpleSynthParams::FILTER_FREQUENCY:		return filter.freq;
			case SimpleSynthParams::FILTER_RESONANCE:		return filter.res;

			default: return 0.0f;
		}
	}

	const std::string getParameterName(Param p) const override {
		SimpleSynthParams pp = (SimpleSynthParams) p;
		switch (pp) {

			case SimpleSynthParams::OSC_HUELL_A:			return "OSC_ATTACK";
			case SimpleSynthParams::OSC_HUELL_D:			return "OSC_DECAY";
			case SimpleSynthParams::OSC_HUELL_S:			return "OSC_SUSTAIN";
			case SimpleSynthParams::OSC_HUELL_R:			return "OSC_RELEASE";

			case SimpleSynthParams::FILTER_HUELL_A:			return "FILTER_ATTACK";
			case SimpleSynthParams::FILTER_HUELL_D:			return "FILTER_DECAY";
			case SimpleSynthParams::FILTER_HUELL_S:			return "FILTER_SUSTAIN";
			case SimpleSynthParams::FILTER_HUELL_R:			return "FILTER_RELEASE";

			case SimpleSynthParams::OSC1_MODE:				return "OSC1_MODE";
			case SimpleSynthParams::OSC2_MODE:				return "OSC2_MODE";

			case SimpleSynthParams::OSC1_OCTAVE:			return "OSC1_OCTAVE";
			case SimpleSynthParams::OSC2_OCTAVE:			return "OSC2_OCTAVE";

			case SimpleSynthParams::OSC1_SEMI:				return "OSC1_SEMI";
			case SimpleSynthParams::OSC2_SEMI:				return "OSC2_SEMI";

			case SimpleSynthParams::OSC1_FINE:				return "OSC1_FINE";
			case SimpleSynthParams::OSC2_FINE:				return "OSC2_FINE";

			case SimpleSynthParams::OSC1_USE_KEYBOARD:		return "OSC1_USE_KEYBOARD";
			case SimpleSynthParams::OSC2_USE_KEYBOARD:		return "OSC2_USE_KEYBOARD";

			case SimpleSynthParams::OSC1_PHASE_OFFSET:		return "OSC1_PHASE_OFFSET";
			case SimpleSynthParams::OSC2_PHASE_OFFSET:		return "OSC2_PHASE_OFFSET";

			case SimpleSynthParams::USE_RING_MODULATION:	return "USE_RING_MODULATION";
			case SimpleSynthParams::OUTPUT_VOLUME:			return "OUTPUT_VOLUME";

			case SimpleSynthParams::OSC_MIX:				return "OSC_MIX";

			case SimpleSynthParams::LFO1_MODE:				return "LFO1_MODE";
			case SimpleSynthParams::LFO1_FREQUENCY:			return "LFO1_FREQUENCY";
			case SimpleSynthParams::LFO1_AMOUNT:			return "LFO1_AMOUNT";

			default:										return "UNKNOWN";

		}
	}

	bool getInputProperties(unsigned int idx, PinProperties* properties) override {
		(void) idx;
		(void) properties;
		return false;
	}

	bool getOutputProperties(unsigned int idx, PinProperties* properties) override {
		switch(idx) {
			case 0:	properties->name = "output (left)";		return true;
			case 1:	properties->name = "output (right)";	return true;
		}
		return false;
	}

	unsigned int getNumParameters() const override {
		return (int) SimpleSynthParams::_END;
	}


	void processNote(Amplitude* output, SimpleSynthNoteDescription& nd, bool stopped, Frequency freq1, Frequency freq2) {

		// calculate frequency for OSC1 / OSC2
		double f1 = nd.note.shift(osc[0].octave * 12 + osc[0].semi).getFrequency() * osc[0].fine;
		double f2 = nd.note.shift(osc[1].octave * 12 + osc[1].semi).getFrequency() * osc[1].fine;

		// LFO
		float curPhaseLfo1 = float(lfo[0].age) * lfo[0].freq * lfo[0].osc.getLUTmultiplier(getSampleRate());
		double lfoImpact = lfo[0].osc.getLUTint( (unsigned int) curPhaseLfo1 ) * lfo[0].amount * 0.1;

		// adjust OSC1 / OSC2 frequency
		f1 += f1 * lfoImpact;
		f2 += f2 * lfoImpact;

		// calculate phase (int value for LUT) increment per sample
		double phaseInc1 = f1 * osc[0].osc.getLUTmultiplier(getSampleRate());
		double phaseInc2 = f2 * osc[1].osc.getLUTmultiplier(getSampleRate());

		// calculate phase-offset (int value for LUT)
		double phaseOffset1 = osc[0].phaseOffset * double(osc[0].osc.getLUTsize());
		double phaseOffset2 = osc[1].phaseOffset * double(osc[1].osc.getLUTsize());


		// adjust the note's filter
		// TODO: may this should happen within the for loop at the cost of much more CPU load
		float filterFreq = filter.freq + filter.adsr.get(stopped, nd.age) * (1.0f - filter.freq);
		nd.bq->setLowPassResonance(filterFreq, filter.res);

		// process each sample
		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {

			// huell-curve factor
			Volume huellVolume = adsr.get(stopped, nd.age + i);
			Volume v = huellVolume * nd.vol;

			// get both amplitudes
			Amplitude a1 = osc[0].osc.getLUTint( (unsigned int) (nd.curPhase1 + phaseOffset1) );
			Amplitude a2 = osc[1].osc.getLUTint( (unsigned int) (nd.curPhase2 + phaseOffset2) );

			// update phases
			nd.curPhase1 += phaseInc1;
			nd.curPhase2 += phaseInc2;

			// modulation?
			Amplitude a;
			if (useRingMod) {
				a = (a1 * a2) * v;
			} else {
				a = (a1 * mix + a2 * (1.0f-mix)) * v;
			}

			// filter
			output[i] += nd.bq->filter(0, a);

		}

		// adjust ages
		nd.age += getSamplesPerProcess();

	}

	void process(Amplitude** inputs, Amplitude** outputs) {

		(void) inputs;

		// remove old fading notes
		notes.fadeout.cleanup(getSampleRate());

		// first: set fixed oscillator frequency
		// those values will be overwritten if note's frequency is determined by keyboard input
//		Frequency freq1 = osc[0].freq * osc[0].fine / getSampleRate();
//		Frequency freq2 = osc[1].freq * osc[1].fine / getSampleRate();
		Frequency freq1 = 0;
		Frequency freq2 = 0;

		//		// calculate final frequency for each active / fading note
		//		for (SimpleSynthNoteDescription& nd : notes.active.getEntries()) {
		//			nd.freq1 = nd.nOsc1.getFrequency() * osc[0].fine  * OSC_LUT_SIZE / getSampleRate();
		//			nd.freq2 = nd.nOsc2.getFrequency() * osc[1].fine  * OSC_LUT_SIZE / getSampleRate();
		//		}
		//		for (SimpleSynthNoteDescription& nd : notes.fadeout.getEntries()) {
		//			nd.freq1 = nd.nOsc1.getFrequency() * osc[0].fine  * OSC_LUT_SIZE / getSampleRate();
		//			nd.freq2 = nd.nOsc2.getFrequency() * osc[1].fine  * OSC_LUT_SIZE / getSampleRate();
		//		}

		//		// process each sample
		//		for (register unsigned int i = 0; i < getSamplesPerProcess(); ++i) {

		//			register Amplitude out = 0;

		// process each to-be-played note
		for (SimpleSynthNoteDescription& nd : notes.active.getEntries()) {
			processNote(outputs[0], nd, false, freq1, freq2);
		}
		for (SimpleSynthNoteDescription& nd : notes.fadeout.getEntries()) {
			processNote(outputs[0], nd, true, freq1, freq2);
		}

		// aging
		lfo[0].age += getSamplesPerProcess();

		//			// mono output
		//			outputs[0][i] = out;

		//		}


		// filtering
		//		filter.bq.setLowPassResonance(filter.freq+0.01, filter.res+0.001);
		//		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
		//			outputs[0][i] = filter.bq.filter(0, outputs[0][i]);
		//		}


		// perform auto gain control to limit the max output
		agc.add(outputs[0], getSamplesPerProcess());
		Volume mul = masterGain;
		if (agc.getMax() > 1.0) {mul /= agc.getMax();}

		vu.reset();
		for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
			outputs[0][i] *= mul;
			outputs[1][i] = outputs[0][i];
			vu.add(outputs[0][i]);
		}

	}

	std::string getProductString() const override {
		return "SimpleSynth";
	}

	void stopNotes() override {
		notes.active.clear();
	}

	void startNote(Note note, Volume vol) override {
		SimpleSynthNoteDescription nd(note, vol);
		notes.active.add(nd);
	}

	void stopNote(Note note) override {

		// get active note, if any
		SimpleSynthNoteDescription* nd = notes.active.get(note);
		if (!nd) {return;}

		// reset age and add note to the fade-out (sustain) vector
		nd->age = 0;
		notes.fadeout.add(*nd);

		// remove note from active vector
		notes.active.remove(note);

	}


	Volume getVU() override {
		return vu.getMax();
	}


	void processEvents(const Events* evts) override {
		(void)evts;
	}

private:

	struct {

		/** currently active notes (Attac, Decay, Sustain) */
		NoteVector<SimpleSynthNoteDescription> active;

		/** currently fading notes (Release) */
		NoteVector<SimpleSynthNoteDescription> fadeout;

	} notes;

	struct State {
		float curPhase1;
		float curPhase2;
		State() : curPhase1(0), curPhase2(0) {;}
	} state;


	struct OSC {

		/** frequency = from current keyboard note. else: fixed */
		bool fromKeyboard;

		/** the octave [0,8] to play, or the offset when using keyboard's note */
		unsigned int octave;

		/** the semi [0,11] to play, or the offset when using keyboard's note */
		unsigned int semi;

		/** the fine to play, or the offset when using keyboard's note */
		double fine;

		/** the phase-offset for this OSC */
		float phaseOffset;

		SimpleOscillator2 osc;
		OSC() : fromKeyboard(true), octave(0), semi(0), fine(1.0f), phaseOffset(0.0f) {;}

	};

	/** settings for the first and second oscillator */
	OSC osc[2];

	/** additional LFOs */
	struct LFO {

		/** the oscillators frequency */
		Frequency freq;

		/** the impact to apply to the target */
		float amount;

		/** the oscillator */
		SimpleOscillator2 osc;

		unsigned int age;

		LFO() : freq(0), amount(0), age(0) {;}

	};

	/** settings for the first and second LFO */
	LFO lfo[2];

	//	struct Plugs {
	//		SimpleOscillator* oscRingMod;
	//		Plugs() : oscRingMod(nullptr) {;}
	//	} plugs;


	/** use ring-modulation? */
	bool useRingMod;

	/** the huell function adjusting the output volume */
	SimpleSynthADSR adsr;


	/** mixing between OSC1 and OSC2 */
	Volume mix;

	/** the output volume */
	Volume masterGain;

	/** vu meter */
	VUCalc vu;

	/** auto gain correction */
	AGC agc;

	/**
	 * filtering params (frequency & resonance)
	 * due to hull-curving, each note has an individual
	 * filter and huell-curve
	 */
	struct Filter {

		/** filter's frequency (between [0,1]) */
		float freq;

		/** filter's resonance (between [0,1]) */
		float res;

		/** the huell function adjusting the filter's frequency */
		SimpleSynthADSR adsr;

		/** ctor */
		Filter() : freq(1), res(0) {;}

	} filter;


};


#endif /* SIMPLESYNTH_H_ */
