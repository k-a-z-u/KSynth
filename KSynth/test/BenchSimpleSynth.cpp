

#ifdef WITH_TESTS
#include "Test.h"

#include "../synth/SimpleSynth.h"
#include <KLib/os/Process.h>


class SynthBench: public ::testing::Test {

public:

	SimpleSynth synth;
	unsigned int cnt = 50;
	float* out[2];
	K::Process proc;
	Note note;

public:

	SynthBench() : proc("/usr/bin/gnuplot"), note(30) {

		synth.setParameter(SimpleSynthParams::OSC1_MODE, 0.1f);
		synth.setParameter(SimpleSynthParams::OSC2_MODE, 0.1f);
		synth.setParameter(SimpleSynthParams::OSC_FM, 0.01f);
		synth.setParameter(SimpleSynthParams::OSC_HUELL_A, 0.50f);
		synth.setParameter(SimpleSynthParams::OSC_HUELL_D, 0.50f);
		synth.setParameter(SimpleSynthParams::OSC_HUELL_S, 0.75f);
		synth.setParameter(SimpleSynthParams::OSC_HUELL_R, 0.01f);
		synth.startNote(note, 0.75f);
		out[0] = new float[synth.getSamplesPerProcess()];
		out[1] = new float[synth.getSamplesPerProcess()];

	}

	void SetUp( ) {

	}

	void TearDown() {
		;
	}

	~SynthBench()  {
		;
	}


};

TEST_F(SynthBench, bench1) {

	unsigned int cnt = 400000;
	for (unsigned int i = 0; i < cnt; ++i) {
		synth.process(nullptr, out);
	}

}

TEST_F(SynthBench, plot) {

	proc << "plot '-' with lines\n";
	for (unsigned int i = 0; i < cnt; ++i) {

		memset(out[0], 0, sizeof(float) * synth.getSamplesPerProcess());
		memset(out[1], 0, sizeof(float) * synth.getSamplesPerProcess());

		if (i == cnt/2) {
			synth.stopNote(note);
		}

		synth.process(nullptr, out);

		for (unsigned int j = 0; j < synth.getSamplesPerProcess(); ++j) {
			proc << out[0][j] << "\n";
		}

	}
	proc << "e\n";
	proc.flush();
	sleep(10);

}





#endif

