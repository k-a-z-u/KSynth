

#ifdef WITH_TESTS
#include "Test.h"

#include "../fx/AllPassReverbGate.h"
#include "../fx/AllPassReverb.h"
#include <KLib/os/Process.h>


/** filter "disabled" */
TEST(AllPassReverbGate, identity) {

	AllPassReverbGate<float, 65536> rev;

	// delay(0) -> 1 sample delay
	float old = 0;
	for (unsigned int i = 0; i < 1024; ++i) {
		float x = (float) rand() / (float) RAND_MAX;
		float y = rev.process(x);
		//std::cout << x << ":" << y << std::endl;
		ASSERT_EQ(old,y);
		old = x;
	}

}

/** no postive/negative gain = just delay the input signal */
TEST(AllPassReverbGate, move) {

	float move = 0.005f;
	unsigned int offset = (unsigned int) (move * 65536);
	AllPassReverbGate<float, 65536> rev;
	rev.setDelay(0.005f);
	rev.setPositiveGain(0.0f);
	rev.setNegativeGain(0.0f);

	// delay(0) -> 1 sample delay
	float in[1024];
	for (unsigned int i = 0; i < 1024; ++i) {
		in[i] = (float) rand() / (float) RAND_MAX;
		float res = rev.process(in[i]);
		if (i <= offset) {
			ASSERT_EQ(0, res);
		} else {
			ASSERT_EQ(in[i-offset-1], res);
		}
	}

}

TEST(AllPassReverbGate, moveAndEcho) {

	float delay = 0.005f;
	AllPassReverbGate<float, 65536> rev;
	unsigned int offset = (unsigned int) (65536 * delay);

	rev.setDelay(delay);
	rev.setPositiveGain(0.5f);
	rev.setNegativeGain(0.0f);

	float out[1024];
	for (unsigned int i = 0; i < 1024; ++i) {
		out[i] = (i == 0) ? (rev.process(1)) : (rev.process(0));
	}

	ASSERT_EQ(0, out[0]);
	ASSERT_EQ(0, out[1]);

	ASSERT_EQ(1.000f, out[1*offset+1]);	// 1st delay
	ASSERT_EQ(0.500f, out[2*offset+2]);	// 2nd delay
	ASSERT_EQ(0.250f, out[3*offset+3]);	// 3rd delay
	ASSERT_EQ(0.125f, out[4*offset+4]);	// 4th delay

}

TEST(AllPassReverbGate, XXX) {

	AllPassReverbGate<float, 65536> rev1;
	AllPassReverbGate<float, 65536> rev2;

	const unsigned int size = 512;

	rev1.setDelay(0.001f);
	rev1.setPositiveGain(0.3f);
	rev1.setNegativeGain(0.1f);

	rev2.setDelay(0.0015f);
	rev2.setPositiveGain(0.3f);
	rev2.setNegativeGain(0.1f);

	K::Process proc("/usr/bin/gnuplot");
	proc << "set yrange[-1.1:1.1]\n";
	proc << "set xrange[-10:*]\n";
	proc << "plot '-' with steps\n";

	for (unsigned int i = 0; i < size; ++i) {
		float res = (i == 0) ? (rev1.process(1)) : (rev1.process(0));
		res = rev2.process(res);
		proc << res << "\n";
	}
	proc << "e\n";

	proc.flush();
	sleep(5);

}

TEST(AllPassReverb, params) {

	AllPassReverb rev;

	for (unsigned int i = 0; i < rev.getNumParameters(); ++i) {
		float rnd = float(rand()) / float(RAND_MAX);
		rev.setParameter(i, rnd);
		ASSERT_NEAR(rnd, rev.getParameter(i), 0.01f);
	}


}


#endif

