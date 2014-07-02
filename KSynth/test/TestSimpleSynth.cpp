#ifdef WITH_TESTS
#include "Test.h"

#include "../synth/SimpleSynth.h"

static float rnd() {
	return float(rand()) / float(RAND_MAX);
}

void testSSParam(SimpleSynth& ss, SimpleSynthParams p, float near) {
	float r = rnd();
	ss.setParameter(p, r);
	ASSERT_NEAR(r, ss.getParameter(p), near);
}


/** new buffer? -> all elements are zero */
TEST(TestSimpleSynth, parameters) {

	SimpleSynth ss;

	testSSParam(ss, SimpleSynthParams::FILTER_FREQUENCY, 0.005f);
	testSSParam(ss, SimpleSynthParams::FILTER_RESONANCE, 0.005f);

	testSSParam(ss, SimpleSynthParams::FILTER_HUELL_A, 0.005f);
	testSSParam(ss, SimpleSynthParams::FILTER_HUELL_D, 0.005f);
	testSSParam(ss, SimpleSynthParams::FILTER_HUELL_S, 0.005f);
	testSSParam(ss, SimpleSynthParams::FILTER_HUELL_R, 0.005f);

	testSSParam(ss, SimpleSynthParams::OSC_HUELL_A, 0.005f);
	testSSParam(ss, SimpleSynthParams::OSC_HUELL_D, 0.005f);
	testSSParam(ss, SimpleSynthParams::OSC_HUELL_S, 0.005f);
	testSSParam(ss, SimpleSynthParams::OSC_HUELL_R, 0.005f);

	testSSParam(ss, SimpleSynthParams::LFO1_AMOUNT, 0.005f);
	testSSParam(ss, SimpleSynthParams::LFO1_FREQUENCY, 0.005f);
	//testSSParam(ss, SimpleSynthParams::LFO1_MODE, 0.005f);

	testSSParam(ss, SimpleSynthParams::OSC1_FINE, 0.005f);
	testSSParam(ss, SimpleSynthParams::OSC1_MODE, 0.2f);
	testSSParam(ss, SimpleSynthParams::OSC1_OCTAVE, 0.2f);
	testSSParam(ss, SimpleSynthParams::OSC1_PHASE_OFFSET, 0.005f);
	testSSParam(ss, SimpleSynthParams::OSC1_SEMI, 0.2f);
	testSSParam(ss, SimpleSynthParams::OSC1_USE_KEYBOARD, 0.5f);

	testSSParam(ss, SimpleSynthParams::OSC2_FINE, 0.005f);
	testSSParam(ss, SimpleSynthParams::OSC2_MODE, 0.2f);
	testSSParam(ss, SimpleSynthParams::OSC2_OCTAVE, 0.2f);
	testSSParam(ss, SimpleSynthParams::OSC2_PHASE_OFFSET, 0.005f);
	testSSParam(ss, SimpleSynthParams::OSC2_SEMI, 0.2f);
	testSSParam(ss, SimpleSynthParams::OSC2_USE_KEYBOARD, 0.5f);

}


#endif

