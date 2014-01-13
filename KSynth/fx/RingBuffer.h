/*
 * RingBuffer.h
 *
 *  Created on: Oct 19, 2013
 *      Author: kazu
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <vector>

template <typename T, int size> class RingBuffer {

public:

	/** ctor */
	RingBuffer() : numUsed(0), head(0), tail(0) {
//		resize(size);
		buffer = (T*) malloc(size * sizeof(T));
		memset(buffer, 0, size * sizeof(T));
	}

	/** dtor */
	~RingBuffer() {
		free(buffer); buffer = nullptr;
	}

//	/** resize the ringbuffer */
//	void resize(unsigned int size) {
//		buffer = (T*) realloc(buffer, size * sizeof(T));
//		this->bufSize = size;
//	}

	/** add a new value to the end of the buffer */
	void push(T val) {
		++head;
		head %= size;
		buffer[head] = val;
		if (numUsed < size) {++numUsed;}
	}

	/** reset the buffer (clear head/tail) */
	void clear() {
		head = 0;
		tail = 0;
	}

	/** return the current head value (most recent one)*/
	T getHead() {
		return buffer[head];
	}

	/** peek for the next entries to fetch. peekBehin(0) returns the tail */
	T peekBehind(int numEntries) {
		unsigned int idx = (head + numEntries);// - numEntries + buffer.size();
		idx %= size;
		return buffer[idx];
	}

	/** peek for entries behind the head. numEntries=0 return the head's value */
	T peekBehindHead(int numEntries) {
		unsigned int idx = ((int)head - numEntries + size) % size;
		return buffer[idx];
	}

	/** get the next value from the buffer */
	T pop() {
		++tail;
		tail %= size;
		T ret = buffer[tail];
		--numUsed;
		return ret;
	}

	/** get the number of used elements */
	unsigned int bytesUsed() {
		return numUsed;
	}

	/**
	 * set the number of used bytes.
	 * this will also move the tail behind the head's postion
	 */
	void setUsed(unsigned int used) {
		this->numUsed = used;
		this->tail = ((int)this->head - used + size) % size;
	}

private:

	/** the number of used bytes */
	unsigned int numUsed;

	/** the next position to insert to */
	unsigned int head;

	/** the next position to fetch from */
	unsigned int tail;

	/** the buffer to use */
	T* buffer;

};


#endif /* RINGBUFFER_H_ */
