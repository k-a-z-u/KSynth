/*
 * RingBuffer.h
 *
 *  Created on: Oct 19, 2013
 *      Author: kazu
 */

#ifndef RINGBUFFERFIXED_H
#define RINGBUFFERFIXED_H

#include <vector>

/**
 * represents a very basic ringbuffer based on a template class.
 * for best performance, the SIZE argument SHOULD be a power of two!
 */
template <typename T, int size> class RingBufferFixed {

public:

	/** ctor */
	RingBufferFixed() : numUsed(0), head(0), tail(0) {
		buffer = (T*) malloc(size * sizeof(T));
		memset(buffer, 0, size * sizeof(T));
	}

	/** dtor */
	~RingBufferFixed() {
		free(buffer); buffer = nullptr;
	}

	/** add a new value to the end of the buffer */
	void push(const T val) {
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

	/** peek for the next entries to fetch. peekBehind(0) returns the tail */
	T peekBehind(const int numEntries) {
		unsigned int idx = (head + numEntries);// - numEntries + buffer.size();
		idx %= size;
		return buffer[idx];
	}

	/** peek for entries behind the head. numEntries=0 return the head's value */
	T peekBehindHead(const int numEntries) {
		unsigned int idx = ((int)head - numEntries + size) % size;
		return buffer[idx];
	}

	/** get the next value from the buffer */
	T pop() {
		++tail;
		tail %= size;
		const T ret = buffer[tail];
		--numUsed;
		return ret;
	}

	/** get the number of used elements */
	unsigned int bytesUsed() const {
		return numUsed;
	}

	/**
	 * set the number of used bytes.
	 * this will also move the tail behind the head's postion
	 */
	void setUsed(const unsigned int used) {
		this->numUsed = used;
		this->tail = ((int)this->head - used + size) % size;
	}

	/** return the ring-buffer's size (from template argument) */
	unsigned int getSize() const {
		return size;
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


#endif // RINGBUFFERFIXED_H
