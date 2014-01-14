/*
 * WaveFile.h
 *
 *  Created on: Oct 27, 2013
 *      Author: kazu
 */

#ifndef WAVEFILE_H_
#define WAVEFILE_H_

#include <KLib/fs/File.h>

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


class WaveFile {

public:

	WaveFile(const K::File& f) {
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

		uint8_t buf[128];
		size_t ret = fread(buf, 1, 128, handle);
		if (ret != 128) {throw WaveFileException("could not read file-header", f);}

		if (memcmp(buf+0, "RIFF", 4) != 0) {throw WaveFileException("does not start with 'RIFF'", f);}
		if (memcmp(buf+8, "WAVE", 4) != 0) {throw WaveFileException("does not start with 'WAVE'", f);}
		if (memcmp(buf+12, "fmt ", 4) != 0) {throw WaveFileException("'fmt' tag missing'", f);}

		uint32_t len = *((uint32_t*) (buf+4)) + 8;
		if (len != f.length()) {throw WaveFileException("header-length does not match file-length", f);}

		len = *((uint32_t*) (buf+16));
		if (len != 16) {throw WaveFileException("header-length ist not 16", f);}

		uint16_t fmt = *((uint16_t*) (buf+20));
		if (fmt != 0x0001) {throw WaveFileException("file-format is not PCM", f);}

		channels = *((uint16_t*) (buf+22));
		sRate = *((uint32_t*) (buf+24));
		bits =  *((uint16_t*) (buf+34));

		if (memcmp(buf+36, "data", 4) != 0) {throw WaveFileException("'data' tag missing'", f);}
		len = *((uint32_t*) (buf+40));

		if (bits != 16) {throw WaveFileException("currently only 16 bits is supported", f);}

		// read the PCM data
		uint8_t* data = (uint8_t*) malloc(len);
		fseek(handle, 44, SEEK_SET);
		size_t bytesRead = fread(data, 1, len, handle);
		if (bytesRead != len)  {throw WaveFileException("could not read data part", f);}

		// parse PCM data
		unsigned int done = 0;
		while (done < len) {
			for (int chan = 0; chan < channels; ++chan) {

				Amplitude a = 0;

				if (bits == 16) {
					int v = *((int16_t*) (data+done)) ;
					a = float(v) / 65535.0f * 2.0f;
					done += 2;
				}

				if (chan == 0) {ch1.push_back(a);}

			}
		}

		free(data);

		fclose(handle);

	}



	SampleRate sRate;

	int channels;

	int bits;

	std::vector<Amplitude> ch1;

};


#endif /* WAVEFILE_H_ */
