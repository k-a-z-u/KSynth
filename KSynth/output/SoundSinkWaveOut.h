#ifdef WITH_WAVE_OUT

#include "SoundSink.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#include <mutex>
#include <queue>
#include <memory>
#include <iostream>


#define WO_FLOAT_TO_UINT16(val)	((val / 2.0) * ((1 << 16) - 1))

/**
 * represents one buffer, bound to the sound-card that is filled by
 * the client, sent to the card, played and an event is provided when
 * the block finishes.
 */
template <typename T> class WaveOutBuffer {

public:

	/** ctor */
	WaveOutBuffer() :
		processed(false), memory(nullptr), maxEntries(0), usedEntries(0) {;}

	/** dtor */
	~WaveOutBuffer() {

		// de-allocate memory
		delete[] memory;

	}

	/** allocate memory for this buffer. change the buffer's size */
	void allocate(unsigned int maxEntries) {

		this->maxEntries = maxEntries;

		// allocate memory (deallocate previous allocations first)
		if (memory) {delete[] memory;}
		memory = new T[maxEntries];

		// zero settings struct
		ZeroMemory(&header, sizeof(WAVEHDR));

		// adjust memory pointer
		header.lpData = (LPSTR) memory;

	}

	/** bind buffer and send it to the sound-card */
	void enqueue(HWAVEOUT hWaveOut) {

		// set buffer's size (number of used bytes)
		header.dwBufferLength = usedEntries * sizeof(T);

		// "bind" the buffer to the sound-card
		waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));

		// send to card
		waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));

		// mark as processed;
		processed = true;

	}

	/** reset this buffer -> unbind from card -> reuseable */
	void reset(HWAVEOUT hWaveOut) {

		// "unbind" header from sound-card
		waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));

		// mark as clean (buffer may now be re-used)
		processed = false;
		usedEntries = 0;

	}

	/** attach new audio-data to this buffer */
	void add(const Amplitude** output, const SampleFrame frames) {

		// interleave audio data
		for (unsigned int i = 0; i < frames; ++i) {
			if (full()) {return;}
			memory[usedEntries++] = WO_FLOAT_TO_UINT16( output[0][i] );
			memory[usedEntries++] = WO_FLOAT_TO_UINT16( output[1][i] );
		}

	}

	/** is the buffer full and ready to be sent to the card? */
	bool full() {
		return usedEntries >= maxEntries;
	}

	/** has this buffer already been sent? */
	bool isProcessed() {
		return processed;
	}

private:

	/** the header's description */
	WAVEHDR header;

	/** has the buffer been sent to the sound-card? */
	bool processed;

	/** the memory for all samples */
	T* memory;

	/** the maximum number of entries (samples) the buffer may hold */
	unsigned int maxEntries;

	/** the number of used entries */
	unsigned int usedEntries;

};


/** called whenever a buffer has been processed by the sound-card */
void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);


/**
 * SoundSink for Windows using the WaveOut API
 * and several buffers to avoid "clicking".
 */
class SoundSinkWaveOut : public SoundSink {

public:

	/** ctor */
	SoundSinkWaveOut(unsigned int numBuffers = 3, unsigned int bufferEntries = 4096) :
		buffersAvailable(numBuffers), curBuffer(0), hWaveOut(nullptr) {

		// create buffers
		buffers.resize(3);
		for (auto& buf : buffers) { buf.allocate(bufferEntries); }

	}

	/** dtor */
	~SoundSinkWaveOut() {
		close();
	}

	void open(AudioFormat fmt) override {

		// setup for 16-bit signed-int audio
		wfx.nSamplesPerSec = fmt.sampleRate;
		wfx.nChannels = 2;
		wfx.cbSize = 0;
		wfx.wBitsPerSample = 16;
		wfx.wFormatTag = WAVE_FORMAT_PCM;
		wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
		wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

		// open sound-card
		MMRESULT ret = waveOutOpen( &hWaveOut, WAVE_MAPPER, &wfx, (DWORD)WaveCallback, (DWORD)this, CALLBACK_FUNCTION);
		if (ret != MMSYSERR_NOERROR) {throw "unable to open audio device";}

	}

	void push(const Amplitude** output, const SampleFrame frames) override {

		// wait for free buffers
		while(!buffersAvailable) {Sleep(10);}

		// clean-up (reset) an old, already processed buffer?
		if (buffers[curBuffer].isProcessed()) {buffers[curBuffer].reset(hWaveOut);}

		// append to current buffer
		buffers[curBuffer].add(output, frames);

		// has the current buffer reached its limit? -> enqueue
		if (buffers[curBuffer].full()) {

			// enqueue for sending to card
			buffers[curBuffer].enqueue(hWaveOut);

			mutex.lock();
			--buffersAvailable;
			mutex.unlock();

			// increment currentBuffer index
			++curBuffer;
			curBuffer %= buffers.size();

		}

	}



	void finalize() override {
		close();
	}

	unsigned int buffersAvailable;

	std::mutex mutex;


private:



	/** close the device */
	void close() {

		if (hWaveOut) {
			waveOutClose(hWaveOut);
			hWaveOut = nullptr;
		}

	}


private:

	/** the buffer currently working on (modulo buffers.size()) */
	unsigned int curBuffer;

	/** all buffers to send to the sound-card (circular buffering) */
	std::vector< WaveOutBuffer<uint16_t> > buffers;

	/** the sound-card */
	HWAVEOUT hWaveOut;

	/** the used sound-format */
	WAVEFORMATEX wfx;

};


void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	(void) hWave;
	(void) dw1;
	(void) dw2;
	SoundSinkWaveOut* ss = (SoundSinkWaveOut*) dwUser;
	if (uMsg == WOM_DONE) {
		ss->mutex.lock();
		++ss->buffersAvailable;
		ss->mutex.unlock();
	}
}

#endif
