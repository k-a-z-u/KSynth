/*
 * WaveFile.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef WAVEFILE_H_
#define WAVEFILE_H_

#include <KLib/fs/File.h>
#include "../../misc/DataTypes.h"

/** exception handling while parsing wave files */
class WaveFileException : public std::exception {
public:
	WaveFileException(const std::string& str) : str(str) {;}
	WaveFileException(const std::string& str, const K::File& file) : str() {
		this->str = "file: " + file.getAbsolutePath() + "\nerror:" + str;
	}
	virtual const char* what() const throw() { return str.c_str(); }
private:
	std::string str;
};


/**
 * a very basic wave-file parser.
 * only supports the PCM format and reads file as a whole
 * (allocs the total memory. no streaming)
 *
 * supports 8, 16 and 24 bits
 *
 */
class WaveFile {

public:

	WaveFile(const K::File& f) : f(f) {
		read(f);
	}

	const std::vector<Amplitude>& get() {return ch1;}

	/** get the wave-file's sample-rate */
	SampleRate getSampleRate() const {return sRate;}

	/** get the number of audio channels */
	Channel getNumChannels() const {return channels;}

private:

	/** read the given wave-file */
	void read(const K::File& f) {

		FILE* handle = fopen(f.getAbsolutePath().c_str(), "rb");
		if (!handle) {throw WaveFileException("error while opening file: " + f.getAbsolutePath());}

		// buffer to read the whole file
		uint32_t fileLen = (uint32_t) f.length();
		uint8_t* data = (uint8_t*) malloc(fileLen);

		// read file completely
		size_t ret = fread(data, 1, fileLen, handle);
		if (ret != fileLen) {throw WaveFileException("could not read the wave file", f);}

		// close
		fclose(handle);

		// check file's beginning
		if (memcmp(data+0, "RIFF", 4) != 0) {throw WaveFileException("does not start with 'RIFF'", f);}
		if (memcmp(data+8, "WAVE", 4) != 0) {throw WaveFileException("does not start with 'WAVE'", f);}
		//if (memcmp(buf+12, "fmt ", 4) != 0) {throw WaveFileException("'fmt' tag missing'", f);}

		// check the length attribute to match the file's length
		uint32_t len = *((uint32_t*) (data+4)) + 8;
		if (len != fileLen) {throw WaveFileException("header-length does not match file-length", f);}

		// now read all chunks within the file
		std::cout << "len: " << fileLen << std::endl;
		uint32_t start = 12;
		while (start < fileLen) {
			std::cout << "start: " << start << std::endl;
			start = readChunk(data, start);
		}

		// cleanup
		free(data);

		std::cout << "DONE" << std::endl;

	}

	/** read the chunk starting at the given position */
	uint32_t readChunk( uint8_t* data, unsigned int start ) {

		// get the chunk's type
		std::string chunkType = std::string( (const char*)(data+start), 4);
		std::cout << "chunk: " << chunkType << std::endl;

		// get the chunk's length
		uint32_t chunkSize = *((uint32_t*) (data+start+4));

		// check what to parse depending on the chunk's type
		if (chunkType == "fmt ") {
			parseFMT(data, start+8, chunkSize);

		} else if (chunkType == "data") {
			parseDATA(data, start+8, chunkSize);

		} else {
			;
		}

		// return the position of the next chunk
		return start + chunkSize + 8;

	}

	/** parse the format block */
	void parseFMT( uint8_t* data, uint32_t start, uint32_t len ) {

		(void) len;

		// we only support PCM format
		uint16_t fmt = *((uint16_t*) (data+start+0));
		if (fmt != 0x0001) {throw WaveFileException("file-format is not PCM", f);}

		// read format params
		channels = *((uint16_t*) (data+start+2));
		sRate = *((uint32_t*) (data+start+4));
		bits =  *((uint16_t*) (data+start+14));

		// sanity checks
		if (bits != 8 && bits != 16 && bits != 24) {throw WaveFileException("bogus bit-value detected", f);}

	}

	/** parse (read) the data chunk containing all samples */
	void parseDATA( uint8_t* data, uint32_t start, uint32_t len ) {

		// parse PCM data
		uint32_t done = 0;
		while (done < len) {
			for (int chan = 0; chan < channels; ++chan) {

				Amplitude a = 0;
				uint32_t pos = start+done;

				if (bits == 8) {
					int v = int ( data[pos+0] << 0 );
					a = Amplitude(v) / Amplitude(128);
					done += 1;

				} else if (bits == 16) {
					int v = int ( (data[pos+0] << 0) | (data[pos+1] << 8) ) << 16 >> 16;
					a = Amplitude(v) / Amplitude(32768);
					done += 2;

				} else if (bits == 24) {
					int v = int ( (data[pos+0] << 0) | (data[pos+1] << 8) | (data[pos+2] << 16) ) << 8 >> 8;
					a = Amplitude(v) / Amplitude(8388608);
					done += 3;

				}

				if (chan == 0) {ch1.push_back(a);}

			}

		}

	}


	/** the loaded file */
	K::File f;

	/** the wave file's sample rate in Hz */
	SampleRate sRate;

	/** the wave file's number of channels */
	int channels;

	/** the wave file's number of bits */
	int bits;

	/** all parsed samples for the first channel */
	std::vector<Amplitude> ch1;

};


#endif /* WAVEFILE_H_ */
