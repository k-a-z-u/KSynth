/*
 * main.cpp
 *
 *  Created on: Oct 23, 2013
 *      Author: kazu
 */


#include "huell/ADSR.h"

#include "oscillator/SimpleOscillator.h"
#include "synth/SimpleSynth.h"
#include <iostream>
#include "notes/Note.h"

#include "output/SoundSinkAlsa.h"
#include "output/SoundSinkWave.h"
#include "output/SoundSinkMp3.h"
#include "output/SoundSinkNull.h"

#include "mixer/SimpleMixer.h"
#include "sampler/SimpleSampler.h"
#include "fx/Reverb.h"
#include "fx/Delay.h"
#include "fx/Flanger.h"
#include "midi/MidiParser.h"
#include "analyzer/FFTAnalyzer.h"
#include "fx/BiquadFilter.h"

#include "Generator.h"
#include "Sequencer.h"

AudioFormat fmt(2, 48000, 16);

//FFTAnalyzer fft(fmt);

BiquadFilter lowpass(fmt);

Reverb reverb(fmt);

//__attribute__((optimize("unroll-loops")))
//inline void doSample(SoundSource& m1, SoundSink& snd, double& sec, AudioFormat& fmt, unsigned int& sample){
//	for (unsigned int i = 0; i < 32; ++i) {
//		AudioData a = m1.get(sec);
//		//a = lowpass.filter(a); a.clamp();
//		//a = reverb.filter(a);
//		//a*= 1.2;
//
//		snd.push(a);
//		//fft.push(a);
//		++sample;
//		sec = (double) sample / (double) fmt.sampleRate;
//	}
//}

struct Track {

	NoteDevice& synth;
	int nextEvent;
	int trackNr;

	MidiEvent evt;

	Track(NoteDevice& synth, int trackNr) :
		synth(synth), nextEvent(0), trackNr(trackNr) {;}

};



#include "test/Test.h"


void fill(float** ret) {
	static float* x[1];
	x[0] = (float*) malloc(4*sizeof(float));
	for (unsigned int i = 0; i < 4; ++i) {
		x[0][i] = i;
	}
	ret[1] = x[0];
}

int main(int argc, char **argv) {



	float* arr[4];
	arr[0] = (float*) malloc(4*sizeof(float));
	arr[1] = (float*) malloc(4*sizeof(float));
	arr[2] = (float*) malloc(4*sizeof(float));
	arr[3] = (float*) malloc(4*sizeof(float));

	fill((float**)&arr[0]);
	for (unsigned int ch = 0; ch < 4; ++ch) {
		for (unsigned int v = 0; v < 4; ++v) {
			std::cout << arr[ch][v] << ",";
		}
		std::cout << std::endl;
	}
	//exit(0);

//#ifdef WITH_TESTS
//	testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();
//#endif

#ifdef OSC_USE_LUT
	OscillatorFunction::initLUTs();
#endif




	lowpass.setNotch(2000,1.5);
	lowpass.reset();
	//lowpass.setBandPass(1000, 1.5);

	//	MidiFile midi;
	//	MidiParser(File("/data/midi/darude_sandstorm.mid"), midi);
	//	exit(0);
	//stk::MidiFileIn mid("/data/midi/superMario/smwwd1.mid"); double speed = 0.3;
	//stk::MidiFileIn mid("/data/midi/superMario/Secretlive.mid");	double speed = 0.5;


	std::vector<unsigned char> vec;

	SoundSinkAlsa snd(fmt);
	//SoundSinkWave snd(File("/tmp/1.wav"), fmt);
	//SoundSinkMp3 snd(File("/tmp/1.mp3"), fmt);
	//SoundSinkNull snd;

	std::vector<Track> tracks;
	SimpleMixer m1;
	m1.setOutputVolume(0.25);

	//Reverb rev(m1, fmt);


	SimpleSampler drums;
	drums.setVolume(0.7);
	drums.addSample(Note(35), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Ac_K.wav")));
	drums.addSample(Note(36), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Ac_K.wav")));

	drums.addSample(Note(38), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Snares/Acu_Snr.wav")));
	drums.addSample(Note(39), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/[KB6]_Roland_D-70_L.A/Snaredrum-05.wav")));
	drums.addSample(Note(40), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Snares/Acu_Snr.wav")));

	//drums.addSample(Note(36), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Ac_H.wav")));

	drums.addSample(Note(49), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Cymbals/Str_Crsh1.wav")));

	drums.addSample(Note(42), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Dol_H1.wav")));
	drums.addSample(Note(46), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Dol_H2.wav")));

	drums.addSample(Note(59), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/[KB6]_Roland_D-70_L.A/Hat Open.wav")));


	// the sequencer to use
	Sequencer seq;


	// PLOT OSCILLATOR FUNCTIONS
	//	Gnuplot gp;
	//	gp << "plot '-' with lines\n";
	//	for (unsigned int i = 0; i < 8000; ++i) {
	//		float x = i / 8000.0;
	//		float y = OscillatorFunction::getSquare(x, 1);
	//		gp << y << "\n";
	//	}
	//	gp << "e\n";
	//	gp.flush();
	//	exit(0);


#define B

#ifdef A

	MidiFile mid;
	MidiParser(File("/data/midi/darude_sandstorm.mid"), mid);
	seq.import(mid);
	seq.setBeatsPerMinute(300);
	//stk::MidiFileIn mid("/data/midi/darude_sandstorm.mid");

	SimpleSynth synth1; synth1.setMode(SimpleOscillatorMode::TRIANGLE, SimpleOscillatorMode::SQUARE); 	synth1.setOsc1FreqOffset(0.997); synth1.setOsc2FreqOffset(1.003);	synth1.setADSR(5,5,0.75,100);	// MEL 1
	SimpleSynth synth2; synth2.setMode(SimpleOscillatorMode::SQUARE, SimpleOscillatorMode::SQUARE);		synth2.setOsc1FreqOffset(0.997); synth2.setOsc2FreqOffset(2.006);	synth2.setADSR(1,1,0.75,3);
	SimpleSynth synth3; synth3.setMode(SimpleOscillatorMode::INV_SAW);									synth3.setOsc1FreqOffset(0.498); synth3.setOsc2FreqOffset(1.002);	synth3.setADSR(0,0,0.75,0);		// BASS

	//	SimpleSampler drums;
	//	drums.setVolume(0.8);
	//	drums.addSample(Note(35), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Ac_K.wav")));
	//	drums.addSample(Note(40), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/[KB6]_Roland_D-70_L.A/Snaredrum-04.wav")));
	//	drums.addSample(Note(39), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/[KB6]_Roland_D-70_L.A/Snaredrum-05.wav")));
	//	drums.addSample(Note(59), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/[KB6]_Roland_D-70_L.A/Hat Open.wav")));

	Flanger fsynth3(fmt);
	fsynth3.setFrequency(1);
	fsynth3.setSource(&synth3);
	fsynth3.setMaxDelay(0.002);
	fsynth3.setFrequency(0.25);

	//Reverb revD(d1, fmt);
	//Reverb rev1(synth1, fmt);
	//Reverb rev2(synth2, fmt);
	//Reverb rev3(synth3, fmt);

	SimpleMixer m2;
	m2.add(&synth1, 0);	m2.setSlotVolume(0, 0.6);
	m2.add(&synth2, 1);		m2.setSlotVolume(1, 0.5);
	m2.add(&fsynth3, 2);		m2.setSlotVolume(2, 0.7);

	Reverb rev1(fmt);
	rev1.setSource(m2);
	rev1.config(3, 0.6361, 0.65);

	Reverb rev2(fmt);
	rev2.setSource(drums);
	rev2.config(3, 0.06, 0.60);

	m1.add(&rev1, 0); m1.setSlotVolume(0, 0.75);
	m1.add(&rev2, 1); m1.setSlotVolume(1, 0.95);
	m1.setOutputVolume(1.1);

	seq.bind(5, &synth1);
	seq.bind(6, &synth2);
	seq.bind(8, &synth3);
	seq.bind(10, &drums);

#endif
#ifdef B

	MidiFile mid;
	MidiParser(File("/data/midi/duke1.mid"), mid);
	//double speed = 0.2;
	seq.import(mid, 0);
	seq.setBeatsPerMinute(85);

	SimpleSynth synth1; synth1.setMode(SimpleOscillatorMode::SAW, SimpleOscillatorMode::SAW);		synth1.setFineOffset(0, 0.5); synth1.setFineOffset(1, 1.002);	synth1.setADSR(1,1,0.75, 1);
	SimpleSynth synth2; synth2.setMode(SimpleOscillatorMode::SQUARE, SimpleOscillatorMode::SQUARE);	synth2.setFineOffset(0, 0.998); synth2.setFineOffset(1, 1.002); synth2.setVolume(0.35); synth2.setADSR(1,1,0.75, 100);
	SimpleSynth synth3; synth3.setMode(SimpleOscillatorMode::SQUARE, SimpleOscillatorMode::SAW);	synth3.setFineOffset(0, 0.998); synth3.setFineOffset(1, 1.002); synth3.setVolume(0.35); synth3.setADSR(1,1,0.75,1);
	SimpleSynth synth4;	synth4.setMode(SimpleOscillatorMode::SQUARE);	synth4.setVolume(0.28); synth4.setADSR(1,1,0.75, 400);

	Flanger f3(fmt);
	f3.setSource(&synth3);
	f3.setMaxDelay(0.001);

	SimpleMixer m2;
	m2.setSlotVolume(0, 0.85);
	m2.setSlotVolume(1, 0.60);
	m2.setSlotVolume(2, 0.55);
	m2.setSlotVolume(3, 0.30);

	m2.bind(0, &synth1);
	m2.bind(2, &synth2);
	m2.bind(4, &synth3);
	m2.bind(6, &synth4);

//	Flanger f4(fmt);
//	f4.setSource(&m2);
//	f4.setMaxDelay(0.002);
//	f4.setFrequency(0.25);
//
//	Reverb rev1(fmt);
//	rev1.setSource(f4);
//	rev1.config(4, 0.8361, 0.65);
//
//	Reverb rev2(fmt);
//	rev2.setSource(drums);
//	rev2.config(4, 0.06, 0.60);

	m1.bind(0, &m2);	m1.setSlotVolume(0, 0.9);
	m1.bind(2, &drums);	m1.setSlotVolume(1, 0.6);
	m1.setOutputVolume(0.85);

	seq.bind(1, &synth1);
	seq.bind(2, &synth2);
	seq.bind(3, &synth3);
	seq.bind(4, &synth4);

	seq.bind(6, &drums);
	seq.bind(7, &drums);
	seq.bind(8, &drums);


#endif
#ifdef C

	SimpleSynth synth1; synth1.setMode(SimpleOscillatorMode::SAW, SimpleOscillatorMode::SQUARE);
	Flanger f1(fmt);
	f1.setSource(&synth1);
	f1.setFrequency(0.5);
	f1.setMaxDelay(0.002);

	synth1.setADSR(0,0,1.0,0);
	//m1.add(&synth1, 0);	m1.setSlotVolume(0, 0.55);
	//m1.add(&f1, 1);	m1.setSlotVolume(0, 0.55);

	synth1.startNote(Note(69-12), 0.0, 1.0);

	stk::MidiFileIn mid("/data/midi/duke1.mid");	double speed = 0.22;

#endif
#ifdef D

	//http://www.zem-college.de/midi/mc_tabed.htm

	MidiFile mid;
	MidiParser(File("/data/midi/tetris.mid"), mid);
	double speed = 0.35;

	SimpleSynth synth1; synth1.setMode(SimpleOscillatorMode::SAW); synth1.setVolume(0.35);
	SimpleSynth synth2; synth2.setMode(SimpleOscillatorMode::SQUARE); synth2.setVolume(0.25);
	SimpleSynth synth3; synth2.setMode(SimpleOscillatorMode::SQUARE); synth3.setVolume(0.25);
	SimpleSynth synth4;	synth4.setMode(SimpleOscillatorMode::SQUARE); synth4.setVolume(0.32); synth4.setADSR(0,0,0.75,10);



	Flanger f1(fmt);
	f1.setSource(&synth1);

	SimpleMixer m2;
	m2.add(&synth1, 0);	m2.setSlotVolume(0, 0.55);
	m2.add(&synth2, 1);	m2.setSlotVolume(1, 0.55);
	m2.add(&synth3, 2);	m2.setSlotVolume(2, 0.55);
	m2.add(&synth4, 3);	m2.setSlotVolume(3, 0.55);

	Reverb rev1(fmt);
	rev1.setSource(m2);
	rev1.config(3, 0.6361, 0.65);

	Reverb rev2(fmt);
	rev2.setSource(drums);
	rev2.config(3, 0.06, 0.60);

	m1.add(&rev1, 0);	m1.setSlotVolume(0, 0.55);
	m1.add(&rev2, 1);	m1.setSlotVolume(1, 0.55);
	m1.setOutputVolume(1.0);

	tracks.push_back(Track(synth1, 2));			// bass
	tracks.push_back(Track(synth2, 3));			// melody 2
	tracks.push_back(Track(synth3, 4));			// melody 1
	tracks.push_back(Track(synth4, 5));			// melody 1

	tracks.push_back(Track(drums, 9));			// snare?
	tracks.push_back(Track(drums, 11));			// kick
	tracks.push_back(Track(drums, 10));			// hihat

#endif
#ifdef E

	MidiFile mid;
	MidiParser(File("/data/midi/devotion.mid"), mid);
	double speed = 0.15;

	SimpleSynth synth1; synth1.setMode(SimpleOscillatorMode::SAW); synth1.setVolume(0.35);
	SimpleSynth synth2; synth2.setMode(SimpleOscillatorMode::SQUARE); synth2.setVolume(0.25);
	SimpleSynth synth3; synth2.setMode(SimpleOscillatorMode::SQUARE); synth3.setVolume(0.25);
	SimpleSynth synth4;	synth4.setMode(SimpleOscillatorMode::SQUARE); synth4.setVolume(0.32); synth4.setADSR(0,0,0.75,10);

	SimpleSampler drums;
	drums.setVolume(0.7);
	drums.addSample(Note(36), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Ac_K.wav")));
	drums.addSample(Note(60), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Ac_K.wav")));
	drums.addSample(Note(0), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Ac_K.wav")));
	drums.addSample(Note(0), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Ac_K.wav")));

	drums.addSample(Note(38), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Snares/Acu_Snr.wav")));
	drums.addSample(Note(40), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Snares/Acu_Snr.wav")));

	//drums.addSample(Note(36), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Ac_H.wav")));

	drums.addSample(Note(49), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Cymbals/Str_Crsh1.wav")));

	drums.addSample(Note(42), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Dol_H1.wav")));
	drums.addSample(Note(46), Sample(File("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Dol_H2.wav")));


	Flanger f1(fmt);
	f1.setSource(&synth1);

	Reverb revD(drums, fmt);
	Reverb rev2(synth2, fmt);

	m1.add(&f1, 0);	m1.setSlotVolume(0, 0.55);
	m1.add(&synth2, 1);	m1.setSlotVolume(1, 0.55);
	m1.add(&synth3, 2);	m1.setSlotVolume(2, 0.55);
	m1.add(&synth4, 3);	m1.setSlotVolume(3, 0.55);
	m1.add(&drums, 7); m1.setSlotVolume(4, 0.8);
	m1.setOutputVolume(1.0);

	tracks.push_back(Track(synth1, 2));			// bass
	tracks.push_back(Track(synth2, 3));			// melody 2
	tracks.push_back(Track(synth2, 4));			// melody 2
	tracks.push_back(Track(synth3, 5));			// melody 1
	tracks.push_back(Track(synth4, 6));			// melody 1
	tracks.push_back(Track(synth4, 7));			// melody 1
	tracks.push_back(Track(synth4, 8));			// melody 1
	tracks.push_back(Track(synth4, 9));			// melody 1

	tracks.push_back(Track(drums, 1));
	//tracks.push_back(Track(drums, 4));
	tracks.push_back(Track(drums, 11));
	tracks.push_back(Track(drums, 12));
	tracks.push_back(Track(drums, 13));
	tracks.push_back(Track(drums, 14));
	tracks.push_back(Track(drums, 15));

#endif
#ifdef F

	MidiFile mid;
	//MidiParser(File("/data/midi/pattern-289.mid"), mid);		seq.import(mid, 0);
	//MidiParser(File("/data/midi/pattern-312.mid"), mid);		seq.import(mid, 0);
	MidiParser(File("/data/midi/pattern-311.mid"), mid);		seq.import(mid, 0);

	seq.setBeatsPerMinute(140);

	SimpleSynth synth1; synth1.setMode(SimpleOscillatorMode::SINE, SimpleOscillatorMode::SINE); synth1.setOsc1FreqOffset(1.996); synth1.setOsc2FreqOffset(1.002); synth1.setADSR(1,300,0.25,150);
	SimpleSynth synth2; synth2.setMode(SimpleOscillatorMode::SQUARE, SimpleOscillatorMode::SQUARE); synth2.setOsc1FreqOffset(0.5); synth2.setOsc2FreqOffset(1.002); synth2.setADSR(1,300,0.25,150);

	SimpleSynth synth3; synth3.setMode(SimpleOscillatorMode::SAW, SimpleOscillatorMode::SAW); synth3.setOsc1FreqOffset(0.496); synth3.setOsc2FreqOffset(0.504); synth3.setADSR(10,100,0.35,20);
	SimpleSynth synth4; synth4.setMode(SimpleOscillatorMode::SINE, SimpleOscillatorMode::SAW); synth4.setOsc1FreqOffset(0.494); synth4.setOsc2FreqOffset(0.5); synth4.setADSR(1,1,0.75,150);

	synth1.setMode(SimpleOscillatorMode::SQUARE); synth1.setOsc1FreqOffset(0.496); synth1.setOsc2FreqOffset(1.002);
	synth2.setMode(SimpleOscillatorMode::SQUARE); synth2.setOsc1FreqOffset(0.996); synth1.setOsc2FreqOffset(1.002);
	synth3.setMode(SimpleOscillatorMode::SAW); synth3.setOsc1FreqOffset(0.996); synth3.setOsc2FreqOffset(1.004);
	synth4.setMode(SimpleOscillatorMode::SAW); synth4.setOsc1FreqOffset(0.998); synth4.setOsc2FreqOffset(1.002);
	//SimpleSynth synth3; synth3.setMode(SimpleOscillatorMode::SAW);
	synth1.setNoteOffset(-12); synth1.setADSR(1,1,0.65,15);
	synth2.setNoteOffset(-12); synth2.setADSR(1,1,0.65,50);
	synth3.setNoteOffset(-12); synth3.setADSR(1,1,0.55,50);
	synth4.setNoteOffset(-12); synth4.setADSR(1,1,0.75,50);


	Flanger f1(fmt);
	f1.setSource(&synth3);
	f1.setMaxDelay(0.001);
	f1.setFrequency(0.3);

	SimpleMixer m2;
	m2.add(&synth1, 0);		m2.setSlotVolume(0, 0.3);
	m2.add(&synth2, 1);		m2.setSlotVolume(1, 0.3);
	m2.add(&f1, 2);		m2.setSlotVolume(2, 1.0);
	m2.add(&synth4, 3);		m2.setSlotVolume(3, 1.0);
	//m2.add(&synth3, 2);		m2.setSlotVolume(2, 0.6);

	Reverb rev1(fmt);
	rev1.setSource(m2);
	rev1.config(4, 0.991, 0.44);

	Reverb rev2(fmt);
	rev2.setSource(drums);
	rev2.config(4, 0.4, 0.50);

	m1.add(&rev1, 0); m1.setSlotVolume(0, 0.75);
	m1.add(&rev2, 1); m1.setSlotVolume(1, 0.95);
	m1.setOutputVolume(1.1);

	PatternSequencer pSeq;
	pSeq.setTarget(&drums);
	pSeq.getPattern(0).bindNote(0, Note(35), 0.75);	//K
	pSeq.getPattern(0).bindNote(2, Note(35), 0.75);	//K
	pSeq.getPattern(0).bindNote(4, Note(38), 0.75);	//S
	pSeq.getPattern(0).bindNote(6, Note(42), 0.75);	//H
	pSeq.getPattern(0).bindNote(7, Note(38), 0.75);	//S

	pSeq.getPattern(0).bindNote(8, Note(35), 0.75);		//K
	pSeq.getPattern(0).bindNote(10, Note(35), 0.75);	//K
	pSeq.getPattern(0).bindNote(12, Note(38), 0.75);	//S
	pSeq.getPattern(0).bindNote(13, Note(38), 0.75);	//S
	pSeq.getPattern(0).bindNote(14, Note(42), 0.75);	//H

	//pSeq.setGranulartity(8);
	//pSeq.setNumBars(2);


	seq.bind(2, &synth3);

	seq.bind(3, &synth3);
	seq.bind(4, &synth4);
	seq.bind(5, &synth4);
	seq.bind(6, &synth2);
	seq.bind(&pSeq);


#endif
#ifdef G

	MidiFile mid;
	MidiParser(File("/data/midi/pattern-318.mid"), mid);		seq.import(mid, 0);

	seq.setBeatsPerMinute(140);

	SimpleSynth synth1; synth1.setMode(SimpleOscillatorMode::SAW, SimpleOscillatorMode::SAW); synth1.setFineOffset(0, 1.996); synth1.setFineOffset(1, 1.002); synth1.setADSR(1,300,0.25,150);
	SimpleSynth synth2; synth2.setMode(SimpleOscillatorMode::SAW, SimpleOscillatorMode::SAW); synth2.setFineOffset(0, 0.5); synth2.setFineOffset(1, 1.002); synth2.setADSR(1,300,0.25,150);

	SimpleSynth synth3; synth3.setMode(SimpleOscillatorMode::SAW, SimpleOscillatorMode::SAW); synth3.setFineOffset(0, 0.496); synth3.setFineOffset(1, 0.504); synth3.setADSR(10,100,0.35,20);
	SimpleSynth synth4; synth4.setMode(SimpleOscillatorMode::SINE, SimpleOscillatorMode::SAW); synth4.setFineOffset(0, 0.494); synth4.setFineOffset(1, 0.5); synth4.setADSR(1,1,0.75,150);

	synth1.setMode(SimpleOscillatorMode::SQUARE); synth1.setFineOffset(0, 0.496); synth1.setFineOffset(1, 1.002);
	synth2.setMode(SimpleOscillatorMode::SQUARE); synth2.setFineOffset(0, 0.996); synth1.setFineOffset(1, 1.002);
	synth3.setMode(SimpleOscillatorMode::SAW); synth3.setFineOffset(0, 0.996); synth3.setFineOffset(1, 1.004);
	synth4.setMode(SimpleOscillatorMode::SAW); synth4.setFineOffset(0, 0.998); synth4.setFineOffset(1, 1.002);
	//SimpleSynth synth3; synth3.setMode(SimpleOscillatorMode::SAW);
	synth1.setNoteOffset(0, -12); synth1.setADSR(1,1,0.65,15);
	synth2.setNoteOffset(0, -12); synth2.setADSR(1,1,0.65,50);
	synth3.setNoteOffset(0, -12); synth3.setADSR(1,1,0.55,50);
	synth4.setNoteOffset(0, -12); synth4.setADSR(1,1,0.75,50);

	BiquadFilter bq1(fmt);
	bq1.setSoundSource(&synth2);
	bq1.setLowPass(1000, 1.5);

	Flanger f1(fmt);
	f1.setSource(&synth3);
	f1.setMaxDelay(0.001);
	f1.setFrequency(0.3);

	SimpleMixer m2;
	//m2.add(&synth1, 0);		m2.setSlotVolume(0, 0.3);
	//m2.add(&bq1, 1);		m2.setSlotVolume(1, 0.3);
	//m2.add(&f1, 2);		m2.setSlotVolume(2, 1.0);
	//m2.add(&synth4, 3);		m2.setSlotVolume(3, 1.0);
	//m2.add(&synth3, 2);		m2.setSlotVolume(2, 0.6);

	m2.bind(0, &synth1);

	Reverb rev1(fmt);
//	rev1.setSource(m2);
	rev1.config(4, 0.991, 0.44);

	Reverb rev2(fmt);
	rev2.setSource(drums);
	rev2.config(4, 0.4, 0.50);

//	m1.add(&rev1, 0); m1.setSlotVolume(0, 0.75);
//	m1.add(&rev2, 1); m1.setSlotVolume(1, 0.95);
	m1.setOutputVolume(1.1);
	m1.bind(0, &synth1);
	m1.bind(2, &synth2);
	m1.bind(4, &synth3);
	m1.bind(6, &synth4);

	PatternSequencer pSeq;
	pSeq.setTarget(&drums);
	pSeq.getPattern(0).bindNote(0, Note(35), 0.75);	//K
	pSeq.getPattern(0).bindNote(2, Note(35), 0.75);	//K
	pSeq.getPattern(0).bindNote(4, Note(38), 0.75);	//S
	pSeq.getPattern(0).bindNote(6, Note(42), 0.75);	//H
	pSeq.getPattern(0).bindNote(7, Note(38), 0.75);	//S

	pSeq.getPattern(0).bindNote(8, Note(35), 0.75);		//K
	pSeq.getPattern(0).bindNote(10, Note(35), 0.75);	//K
	pSeq.getPattern(0).bindNote(12, Note(38), 0.75);	//S
	pSeq.getPattern(0).bindNote(13, Note(38), 0.75);	//S
	pSeq.getPattern(0).bindNote(14, Note(42), 0.75);	//H

	//seq.bind(1, &synth1);
	seq.bind(1, &synth1);

	seq.bind(2, &synth2);
	seq.bind(&pSeq);


#endif


	Generator gen(fmt);
	gen.setSequencer(&seq);
	gen.setSource(&m1);
	gen.setSink(&snd);
	gen.start();


	while(true) {
		usleep(100000);
		if (seq.isDone()) {break;}
	}
	usleep(1000*50);

	gen.stop();

	/*
	unsigned int sample = 0;
	double sec = 0;

	while(true) {

		// get current position in milliseconds
		int midiMs = sec * 1000.0 * speed;


		// process each track
		for (Track& track : tracks) {

			if (midiMs > track.nextEvent) {

				again:;

				// convert midi note nr to Hz
				//double freq = track.next.n.getFrequency();
				std::cout << "track(" << track.trackNr << ")\t";
				std::cout << "time(" << track.nextEvent << ")\t";
				std::cout << "cmd(" << (int) track.evt.getType() << ")\t";
				std::cout << "d1(" << (int) track.evt.d1 << ")\t";
				std::cout << "d2(" << (int) track.evt.d2 << ")\t";
				std::cout << std::endl;
				//std::cout << "note(" << Note(track.next.n.getNoteNr() <<  ", " << freq << " Hz)\t";
				//std::cout << "vol(" << track.next.vol << ")" << std::endl;

				if (track.evt.status) {
					if (track.evt.getType() == MidiEventType::NOTE_OFF) {
						//|| (track.evt.getType() == MidiEventType::NOTE_ON && track.evt.d2 == 0) ) {
						track.synth.stopNote( Note(track.evt.d1), sec);
						std::cout << "stop" << std::endl;
					} else if (track.evt.getType() == MidiEventType::NOTE_ON) {
						track.synth.startNote( Note(track.evt.d1), sec, track.evt.d2 / 127.0);
						std::cout << "start" << std::endl;
					}
				}
				//} else if (track.next.cmd == 176) {
				//track.synth.startNote(track.next.n, sec, track.next.vol);
				//}

				// get next event
				track.evt = mid.getTrack(track.trackNr).popEvent();

				if (!track.evt.status) {std::cout << "EOT?" << std::endl; track.nextEvent += 10000000; continue;}

				// multiple events?
				if (!track.evt.delay) {goto again;}

				//if (track.evt.delay > 200) {track.evt.delay = 200;}

				// shedule next event
				track.nextEvent += track.evt.delay;

			}

		}


		// sample
		doSample(m1, snd, sec, fmt, sample);
		//if ( (t.sample % 4096) == 0) {
		//	usleep(100);
		//}

		if (midiMs > 60*1000) {break;}

	}
	 */


	snd.finalize();

	return 0;

}


