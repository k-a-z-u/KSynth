/*
 * SoundSinkWave.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef SOUNDSINKWAVE_H_
#define SOUNDSINKWAVE_H_

#include <KLib/fs/File.h>
#include "../misc/AudioFormat.h"
#include <cstdint>
#include <fstream>

class BinaryStream {

public:

	BinaryStream(K::File f) {
		out = new std::ofstream(f.getAbsolutePath(), std::ios::binary);
	}

	~BinaryStream() {
		if (out) {out->close(); delete out; out = nullptr;}
	}

	void write(const char* str) {
		*out << str;
	}

	void write(uint8_t v) {
		*out << v;
	}

	void write(uint32_t v) {
		out->write((const char*)(&v), 4);
	}

	void write(uint16_t v) {
		out->write((const char*)(&v), 2);
	}

	void seek(uint32_t p) {
		out->seekp(p);
	}

	void reset() {
		seek(44);
	}

	void flush() {
		out->flush();
	}


private:

	std::ofstream* out;

};

/**
 * write sound to wave file
 */
class SoundSinkWave : public SoundSink {

public:

	/** ctor */
	SoundSinkWave(K::File f, AudioFormat fmt) : out(f), fmt(fmt), numBytes(0) {
		out.reset();
	}

	/** dtor */
	~SoundSinkWave() {
		;
	}

	void open() override {
		numBytes = 0;
		out.reset();
	}

	void push(const Amplitude** outputs, const SampleFrame frames) override {

		for (SampleFrame i = 0; i < frames; ++i) {

			numBytes += fmt.bits/8;

			uint16_t sLeft = amplitudeToI16(outputs[0][i]);
			uint16_t sRight = amplitudeToI16(outputs[1][i]);

			if (fmt.numChannels == 1) {
				out.write(sLeft);
			} else if (fmt.numChannels == 2) {
				out.write(sLeft);
				out.write(sRight);
			}

		}

	}

	void finalize() override {
		writeHeader();
	}

private:

	/** construct the wave header */
	void writeHeader() {

		out.seek(0);

		out.write("RIFF");
		out.write((uint32_t) (numBytes + 44 - 8) );	// file-size including FOLLOWING header
		out.write("WAVE");

		out.write("fmt ");
		out.write((uint32_t) (16));					// 16 bytes following
		out.write((uint16_t) (0x0001));				// PCM
		out.write((uint16_t) (fmt.numChannels));	// the numnber of channels
		out.write((uint32_t) (fmt.sampleRate));		// the sample-rate in Hz
		out.write((uint32_t) (fmt.sampleRate * fmt.numChannels * fmt.bits / 8));	// avg data-rate
		out.write((uint16_t) (fmt.numChannels * ((fmt.bits + 7) / 8)));			// frame-size
		out.write((uint16_t) (fmt.bits));			// bits per sample

		out.write("data");
		out.write((uint32_t) (numBytes));			// data block size

		out.flush();

	}

	BinaryStream out;

	AudioFormat fmt;

	unsigned int numBytes;
};


#endif /* SOUNDSINKWAVE_H_ */
