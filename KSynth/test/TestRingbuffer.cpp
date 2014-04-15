/*
 * TestRingbuffer.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: kazu
 */




#ifdef WITH_TESTS
#include "Test.h"

#include "../fx/RingBuffer.h"

TEST(Ringbuffer, PushPop) {

	const int SIZE = 0xFFFF+1;
	const int MAX = 0x123;

	RingBuffer<int, SIZE> rb;
	int cnt = 0;

	for (unsigned int rep = 0; rep < 1000; ++rep) {

		// push
		for (unsigned int i = 1; i <= MAX; ++i) {
			rb.push(i);
			if (++cnt <= SIZE) {
				ASSERT_EQ(i, rb.bytesUsed());
			}
		}

		// pop
		for (unsigned int i = 1; i <= MAX; ++i) {
			ASSERT_EQ(i, rb.pop());
		}
	}

}

TEST(Ringbuffer, peek) {

	RingBuffer<int, 0xFFFF+1> rb;

	for (unsigned int j = 0; j < 2000; ++j) {

		for (unsigned int i = 0; i <=100; ++i) {
			rb.push(i);
		}

		EXPECT_EQ(100, rb.peekBehindHead(0));
		EXPECT_EQ(99, rb.peekBehindHead(1));
		EXPECT_EQ(75, rb.peekBehindHead(25));
		EXPECT_EQ(50, rb.peekBehindHead(50));
		EXPECT_EQ(25, rb.peekBehindHead(75));
		EXPECT_EQ(0, rb.peekBehindHead(100));

	}

}

#endif
