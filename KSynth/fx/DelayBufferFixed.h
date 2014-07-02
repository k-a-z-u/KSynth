#ifndef DELAYBUFFERFIXED_H
#define DELAYBUFFERFIXED_H

/**
 * basic delay buffer (using a ring-buffer structure)
 * to delay the input by a given amount of samples.
 *
 * size SHOULD be a power of two!
 */
template <typename type, int size> class DelayBufferFixed {

public:

	/** ctor */
	DelayBufferFixed() : buffer(nullptr), headIdx(0), tailIdx(0), delay(0) {
		buffer = new type[size];
		reset();
	}

	/** copy ctor */
	DelayBufferFixed(const DelayBufferFixed& other) : buffer(nullptr), headIdx(0), tailIdx(0), delay(0) {
		buffer = new type[size];
		reset();
		headIdx = other.headIdx;
		tailIdx = other.tailIdx;
		delay = other.delay;
	}

	/** reset the delay buffer by writing zeros to it */
	void reset() {
		memset(buffer, 0, sizeof(type) * size);
	}

	/** dtor */
	~DelayBufferFixed() {
		if (buffer) {delete[] buffer; buffer = nullptr;}
	}

	/** set the delay to apply in "number of entries" */
	void setDelay(unsigned int delay) {
		this->delay = delay;
		tailIdx = (headIdx - delay + size) % size;
	}

	/** push a new value into the buffer */
	void push(type value) {
		headIdx = (headIdx + 1) % size;
		buffer[headIdx] = value;
	}

	/** get the tail's current value */
	type pop() {
		tailIdx = (tailIdx + 1) % size;
		return buffer[tailIdx];
	}

	/** get the buffers max delay */
	unsigned int getMaxDelay() const {
		return size;
	}

	/** get the current delay */
	unsigned int getDelay() const {
		return this->delay;
	}

private:

	/** assignment op */
	void operator = (const DelayBufferFixed& other);

	/** the buffered data goes here */
	type* buffer;

	/** index to the head element */
	unsigned int headIdx;

	/** index to the tail element */
	unsigned int tailIdx;

	/** just a store else we would have to calculate this value */
	unsigned int delay;

};

#endif // DELAYBUFFERFIXED_H
