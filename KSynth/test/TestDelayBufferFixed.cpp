
/*
 * TestDelay.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: kazu
 */


#ifdef WITH_TESTS
#include "Test.h"

#include "../fx/DelayBufferFixed.h"

/** new buffer? -> all elements are zero */
TEST(DelayBufferFixed, zero) {

	DelayBufferFixed<float, 65536> buf;
	buf.setDelay(0);

	for (unsigned int i = 0; i < 65536; ++i) {
		ASSERT_EQ(0, buf.pop());
	}

}

/** no delay? -> input/output match */
TEST(DelayBufferFixed, noDelay) {

	DelayBufferFixed<float, 65536> buf;
	buf.setDelay(0);

	for (unsigned int i = 0; i < 1024; ++i) {
		buf.push( (float) i );
		ASSERT_EQ(i, buf.pop());
	}

}

TEST(DelayBufferFixed, delay1) {

	unsigned int delay = 10;
	float in[1024];

	DelayBufferFixed<float, 65536> buf;
	buf.setDelay(delay);

	for (unsigned int i = 0; i < 1024; ++i) {
		float f = (float) i;
		in[i] = f;
		buf.push(f);
		if ( i >= delay ) {
			ASSERT_EQ(in[i-10], buf.pop());
		} else {
			ASSERT_EQ(0, buf.pop());
		}
	}

}

/** pop before push */
TEST(DelayBufferFixed, delayPopPush) {

	unsigned int delay = 100;
	float in[1024];

	DelayBufferFixed<float, 65536> buf;
	buf.setDelay(delay);

	for (unsigned int i = 0; i < 1024; ++i) {

		float old = buf.pop();

		float f = (float) i;
		in[i] = f;
		buf.push(f);

		if ( i >= delay ) {
			ASSERT_EQ(in[i-100], old);
		} else {
			ASSERT_EQ(0, old);
		}
	}

}


TEST(DelayBufferFixed, delayStream) {

	unsigned int delay = 10;
	float in[65536];

	DelayBufferFixed<float, 1024> buf;
	buf.setDelay(delay);

	for (unsigned int i = 0; i < 65536; ++i) {
		float f = (float) i;
		in[i] = f;
		buf.push(f);
		if ( i >= delay ) {
			ASSERT_EQ(in[i-10], buf.pop());
		} else {
			ASSERT_EQ(0, buf.pop());
		}
	}

}

#endif

