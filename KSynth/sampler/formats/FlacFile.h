/*
 * FlacFile.h
 *
 *  Created on: Dec 29, 2013
 *      Author: kazu
 */

#ifndef FLACFILE_H_
#define FLACFILE_H_

#ifdef WITH_FLAC

#include <FLAC/all.h>
//#include "../../lib/flac/all.h"
//#include "../../lib/flac/decode.h"
#include <klib/streams/FileInputStream.h>

typedef enum { RAW, WAV, AIF, FLAC, OGGFLAC } FileFormat;

class FlacFile {

public:

	FlacFile(const K::File& f) {
		open(f);
	}

	/** get the PCM data for the given channel */
	const std::vector<Amplitude>& get(unsigned int channel) const {
		return state.pcm;
	}

	/** get the file's sample rate */
	SampleRate getSampleRate() const {
		return state.srate;
	}

	/** get the number of used channels */
	Channel getNumChannels() const {
		return state.channels;
	}

private:

	/** the current decoding state */
	struct FlacState {
		K::FileInputStream* fis;
		std::vector<float> pcm;
		SampleRate srate;
		Channel channels;
	} state;



	void open(const K::File& f) {

		state.fis = new K::FileInputStream(f);

		// create a new decoder
		FLAC__StreamDecoder* dec = FLAC__stream_decoder_new();

		// initialize the decoder
		FLAC__stream_decoder_init_stream(dec, &readCallback, nullptr, nullptr, nullptr, nullptr, &writeCallback, nullptr, &errorCallback, (void*)&state);

		// decode completely
		FLAC__stream_decoder_process_until_end_of_stream(dec);

		// cleanup
		FLAC__stream_decoder_finish(dec);
		FLAC__stream_decoder_delete(dec);

		delete state.fis;

	}


	/** ReadCallback: provide data to flac */
	static FLAC__StreamDecoderReadStatus readCallback(const FLAC__StreamDecoder* decoder, FLAC__byte buffer[], size_t* bytes, void* client_data) {

		(void) decoder;

		// fetch client data-structure (state)
		FlacState* state = (FlacState*) client_data;

		int read = state->fis->read(buffer, (unsigned int) *bytes);
		if (read == -1) {
			bytes = 0;
			return FLAC__StreamDecoderReadStatus::FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
		}
		*bytes = read;
		return FLAC__StreamDecoderReadStatus::FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;

	}

	/** WriteCallback: receive decoded raw data */
	static FLAC__StreamDecoderWriteStatus writeCallback(const FLAC__StreamDecoder* decoder, const FLAC__Frame* frame, const FLAC__int32* const buffer[], void* client_data) {

		(void) decoder;

		// fetch client data-structure (state)
		FlacState* state = (FlacState*) client_data;

		// number of samples per channel
		state->srate = frame->header.sample_rate;
		state->channels = frame->header.channels;

		// the maxium value
		unsigned int maxVal = (1 << frame->header.bits_per_sample);

		if (buffer) {
			for (Channel c = 0; c < state->channels; ++c) {
				if (!buffer[c]) {continue;}
				if (c != 0) {continue;}		// FIXME
				for (unsigned int i = 0; i < frame->header.blocksize; ++i) {
					Amplitude a = Amplitude(buffer[c][i]) / Amplitude(maxVal/2);
					state->pcm.push_back(a);
				}
			}
		}

		// proceed
		return FLAC__StreamDecoderWriteStatus::FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;

	}

	/** ErrorCallback */
	static void errorCallback(const FLAC__StreamDecoder* decoder, FLAC__StreamDecoderErrorStatus status, void* client_data) {
		(void) decoder;
		(void) status;
		(void) client_data;
		throw "error while decoding FLAC file";
	}


};

#endif

#endif /* FLACFILE_H_ */
