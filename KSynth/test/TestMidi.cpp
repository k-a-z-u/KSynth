
/*
 * TestMidi.cpp
 *
 *  Created on: Apr 05, 2014
 *      Author: kazu
 */


#ifdef WITH_TESTS
#include "Test.h"

#include "../midi/MidiFile.h"
#include "../midi/MidiParser.h"

TEST(Midi, parse) {

	std::cout << "run" << std::endl;
	K::File f("/apps/workspaces/KSynth/_build/ylvis-the_fox_what_does_the_fox_say.mid");
	MidiFile mf;
	MidiParser p(f, mf);

	std::cout << "Version: " << (int)mf.getVersion() << std::endl;

}


#endif

