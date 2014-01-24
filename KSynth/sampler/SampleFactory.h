/*
 * SampleFactory.h
 *
 *  Created on: Dec 29, 2013
 *      Author: kazu
 */

#ifndef SAMPLEFACTORY_H_
#define SAMPLEFACTORY_H_

#include "Sample.h"
#include "formats/WaveFile.h"
#include "formats/FlacFile.h"


class SampleFactoryException : public std::exception {
public:
	SampleFactoryException(const std::string& str) : str(str) {;}
	SampleFactoryException(const std::string& str, const K::File& file) : str() {
		this->str = "file: " + file.getAbsolutePath() + "\nerror:" + str;
	}
	virtual const char* what() const throw() { return str.c_str(); }
private:
	std::string str;
};


/**
 * load sample from a given file.
 * distinguishes between .wav, .flac, ....
 */
class SampleFactory {

public:

	/**
	 * load sample from the given file.
	 * if the file's type is not supported, this will throw an exception.
	 * @param file the file to load
	 * @param targetSampelRate the sample rate to use for this file (uses conversion if necessary)
	 */
	static Sample load(const K::File& file, SampleRate targetSampelRate = 0) {

		// sanity check
		if (!file.exists()) {
			throw SampleFactoryException("file not found:", file);
		}

		// get "magic bytes"
		uint8_t buf[128];
		peek(file, buf, 128);

		// get format
		Sample s;
		if		(buf[0] == 'R' && buf[1] == 'I' && buf[2] == 'F' && buf[3] == 'F')	{loadWave(file, s);}
		else if	(buf[0] == 'f' && buf[1] == 'L' && buf[2] == 'a' && buf[3] == 'C')	{loadFlac(file, s);}
		else																		{throw SampleFactoryException("unsupported file format!");}

		// convert sample-rate?
		if (targetSampelRate) {
			s.convertTo(targetSampelRate);
		}

		// done
		return s;

	}

private:

	/** load sample from wave file */
	static void loadWave(const K::File& f, Sample& s) {
		WaveFile wf(f);
		s = Sample(wf.get(), wf.getSampleRate(), wf.getNumChannels());
	}

	static void loadFlac(const K::File& f, Sample& s) {
#ifdef WITH_FLAC
		FlacFile ff(f);
		s = Sample(ff.get(0), ff.getSampleRate(), ff.getNumChannels());
#else
		(void) f;
		(void) s;
        throw SampleFactoryException("flac support not compiled!");
#endif
	}

	/** read the first few bytes of a file to distinguish the format */
	static void peek(const K::File& f, uint8_t* buf, int len) {

		FILE* handle = fopen(f.getAbsolutePath().c_str(), "rb");
		//if (!handle) {throw ("error while opening file: " + f.getAbsolutePath());}
		size_t ret = fread(buf, 1, len, handle);
		(void) ret;
		fclose(handle);

	}

};


#endif /* SAMPLEFACTORY_H_ */
