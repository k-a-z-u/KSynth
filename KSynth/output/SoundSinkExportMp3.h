/*
 * SoundSinkMp3.h
 *
 *  Created on: Nov 9, 2013
 *      Author: kazu
 */

#ifndef SOUNDSINKMP3_H_
#define SOUNDSINKMP3_H_

#ifdef WITH_LAME

#include "SoundSinkExport.h"
#include <lame/lame.h>
#include <cstdio>

/** buffer for PCM and mp3 */
#define SSM3_BUF	16384

/**
 * write incoming sound-data to a MP3.
 * this implementation uses LAME
 */
class SoundSinkMp3 : public SoundSinkExport {

public:

	/** ctor */
	SoundSinkMp3() {
		;
	}

	/** dtor */
	~SoundSinkMp3() {
		close();
	}

	void setFile(const K::File &file) override {
		this->f = file;
	}

	void close() {

		if (lame) {
			lame_close(lame);
			lame = nullptr;
		}

		if (out) {
			fclose(out);
			out = nullptr;
		}

	}

	void open(AudioFormat fmt) override {

		// configure MP3 output
		lame = lame_init();
		lame_set_in_samplerate(lame, fmt.sampleRate);
		lame_set_num_channels(lame, fmt.numChannels);
		//lame_set_lowpassfreq(lame, 19000);
		//lame_set_VBR(lame, vbr_default);
		//lame_set_mode(lame, JOINT_STEREO);
		lame_set_preset(lame, 3);
		lame_init_params(lame);


		// open output file
		out = fopen(f.getAbsolutePath().c_str(), "wb");

	}

	void push(const Amplitude** outputs, const SampleFrame frames) override {

		// output
		uint8_t mp3Buf[SSM3_BUF];

		// encode chunk
		int written = lame_encode_buffer_ieee_float(lame, outputs[0], outputs[1], frames, mp3Buf, SSM3_BUF);

		// write to file
		fwrite(mp3Buf, written, 1, out);

	}


	void finalize() override {
		//flushBuffer();
		finalizeMp3();
		close();
	}

	std::string getName() override {
		return "mp3 file (*.mp3)";
	}

private:


//	/** flush current PCM buffer to MP3 */
//	void flushBuffer(SampleFrame frames) {
//		uint8_t mp3Buf[SSM3_BUF];
//		//int written = lame_encode_buffer_interleaved(lame, pcmBuf, pcmBufUsed/sizeof(int16_t), mp3Buf, SSM3_BUF);
//		lame_encode_buffer_ieee_float(lame, )
//		int written = lame_encode_buffer_interleaved_ieee_float(lame, pcmBuf, frames, mp3Buf, SSM3_BUF);
//		fwrite(mp3Buf, written, 1, out);
//		pcmBufUsed = 0;
//	}

	/** finalize the MP3 (flush pending data) */
	void finalizeMp3() {
		uint8_t mp3Buf[SSM3_BUF];
		int written = lame_encode_flush(lame, mp3Buf, SSM3_BUF);
		fwrite(mp3Buf, written, 1, out);
	}


	/** the file to write to */
	K::File f;

	/** store pcm data here */
	//float pcmBuf[SSM3_BUF*2];
	//int pcmBufUsed;

	/** the lame output handle */
	lame_t lame;

	/** output file handle */
	FILE* out;

};

#endif

#endif /* SOUNDSINKMP3_H_ */
