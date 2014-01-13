/*
 * TestDelay.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: kazu
 */


#ifdef WITH_TESTS
#include "Test.h"

#include "../fx/Delay.h"

TEST(Delay, run) {

	const int srate = 48000;
	const float delay = 0.01;
	const int behind = srate * delay;

	AudioFormat fmt(2, srate, 16);
	Delay d(fmt);
	//d.setDelayImmediately(delay);
	d.setDelay(delay);

	for (unsigned int i = 0; i < 1000; ++i) {
		AudioData ad1(i);
		AudioData ad2 = d.filter(ad1);
		EXPECT_EQ(ad1.left , ad2.left + behind);
	}


}


#endif

