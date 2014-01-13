/*
 * MidiParser.h
 *
 *  Created on: Nov 1, 2013
 *      Author: kazu
 */

#ifndef MIDIPARSER_H_
#define MIDIPARSER_H_

#include <klib/fs/File.h>
#include <klib/streams/Endian.h>

class MidiParserException : public std::exception {
public:
	MidiParserException(const std::string& str, const K::File& file) : str() {
		this->str = "error:\t" + str + "\nfile:\t" + file.getAbsolutePath();
	}
	virtual const char* what() const throw() { return str.c_str(); }
private:
	std::string str;
};

#include "MidiFile.h"
#include "MidiEvent.h"

/**
 * parse midi files
 */
class MidiParser {

public:

	/** ctor */
	MidiParser(const K::File& f, MidiFile& dst) :
		file(f), dst(dst), buf(nullptr), head(nullptr), fileLenght(0), numTracks(0) {
		read();
		parse();
	}

	/** dtor */
	~MidiParser() {
		if (buf) {free(buf); buf = nullptr;}
	}


private:

	/** read the file completely */
	void read() {

		// open
		FILE *fh = fopen(file.getAbsolutePath().c_str(), "rb");
		if (fh == nullptr) {throw MidiParserException("could not open midi file", file);}

		// seek to the file's end to get the file's length
		fseek(fh, 0, SEEK_END);
		fileLenght = (unsigned int) ftell(fh);
		rewind(fh);
		buf = (uint8_t*) malloc(fileLenght);
		if (buf == nullptr) {throw MidiParserException("out of memory while reading midi file", file);}

		// read file
		size_t res = fread(buf, fileLenght, 1, fh);
		if (res != 1) {throw MidiParserException("error while reading midi file", file);}

		// cleanup
		fclose(fh);

	}

	/** parse the midi file */
	void parse() {

		// check signature
		if (memcmp(buf+0, "MThd", 4)) {throw MidiParserException("this is not a midi file", file);}

		// read the whole file
		head = buf;
		do {
			readChunk();
		} while ((head-buf) < fileLenght);

	}

	/** read the next chunk within the file */
	void readChunk() {

		// type of the chunk
		std::string type((const char*)(head), 4);			head += 4;

		// length of the chunk
        uint32_t length = K::Endian::bigToHost( *((uint32_t*)(head)) );	head += 4;

		uint8_t* oldHead = head;

		// what to do
		if		(type == "MThd")	{readHeader(length);}
		else if	(type == "MTrk")	{readTrack(length);}
		else						{throw MidiParserException("found unknown chunk type: '" + type + "'", file);}

		// done
		// this will ensure reading properly even when reading
		// a chunk fails to read
		head = oldHead + length;

	}

	/** parse the midi header */
	void readHeader(int length) {

		std::cout << "header" << std::endl;
		if (length != 6) {throw MidiParserException("header length != 6 is currently not supported", file);}

		// midi type
        midiType = K::Endian::bigToHost( *((uint16_t*)(head)) );			head += 2;
		if (midiType > 1) {throw MidiParserException("midi-type != 0/1 is currently not supported", file);}

		// number of tracks
        numTracks = K::Endian::bigToHost( *((uint16_t*)(head)) );			head += 2;

		// speed
        dst.speed = K::Endian::bigToHost( *((uint16_t*)(head)) );			head += 2;

		// if midi type is v0, the file must contain only ONE track! (v1 contains one track per channel)
		if (midiType == 0 && numTracks != 1) {
			throw MidiParserException("midi-type is 0 but file contains more than one channel!", file);
		}

	}

	/** parse data for one track */
	void readTrack(int length) {
		if (midiType == 0)	{readTrackV0(length);}
		else 				{readTrackV1(length);}
	}


	/**
	 * midi V0 contains only one track.
	 * for better editing we will create one track per channel
	 * and split events into multiple tracks (max 16)
	 */
	void readTrackV0(int length) {

		MidiTrack track[16];

		// check the length of the track data
		uint8_t* start = head;

		do {

			MidiEvent evt;

			// the delay of the event
			evt.delay = readVarLen();

			// peek into the even type to distinguish
			// what to do next
			evt.status = *head;

			if		( (evt.status&0xFF) == 0xFF)	{++head; readTrackMeta(track[0]);}
			else if	( (evt.status&0xF0) == 0xF)		{++head; readTrackSystem();}
			else									{
				readTrackOther(evt);
				unsigned int chan = evt.getChannel();
				track[chan].add(evt);
			}

		} while ((head-start) < length);

		// append tracks to file (skip empty ones
		for (unsigned int i = 0; i < 16; ++i) {
			if (track[i].isEmpty()) {continue;}
			dst.add(track[i]);
		}

	}

	/** parse data for one track */
	void readTrackV1(int length) {

		MidiTrack track;

		// check the length of the track data
		uint8_t* start = head;

		do {

			MidiEvent evt;

			// the delay of the event
			evt.delay = readVarLen();

			// peek into the even type to distinguish
			// what to do next
			evt.status = *head;

			if		( (evt.status&0xFF) == 0xFF)	{++head; readTrackMeta(track);}
			else if	( (evt.status&0xF0) == 0xF)		{++head; readTrackSystem();}
			else									{readTrackOther(evt); track.add(evt);}

		} while ((head-start) < length);

		// append track to file
		dst.add(track);

	}

	void readTrackMeta(MidiTrack& dst) {
		std::cout << " meta:\t";
		uint8_t type = *head++;
		int len = readVarLen();
		if		(type == 0x2F && len == 0x00) {std::cout << "end of track" << std::endl;}
		else if	(type == 0x54 && len == 0x05) {
			std::cout << "SMPTE offset" << std::endl;
		} else if (type == 0x58 && len == 0x04) {
			std::cout << "Time signature" << std::endl;
		} else if (type == 0x59 && len == 0x02) {
			std::cout << "Key signature" << std::endl;
		} else if (type == 0x51 && len == 0x03) {
			std::cout << "Tempo" << std::endl;
		} else if (type == 0x3) {
			dst.name = std::string( (const char*)(head), len );
		} else {
			std::cout << "(" << (int)type << ")\t";
			std::string content((const char*)(head), len);
			std::cout << '"' << content << '"' << std::endl;
		}
		head += len;
	}

	// TODO!
	void readTrackSystem() {
		std::cout << " system:\t";
		int len = readVarLen();
		std::cout << "len:" << (int)len << std::endl;
		head += len;
	}

	/** parse normal track events and store them within the given data structure */
	void readTrackOther(MidiEvent& evt) {

		//std::cout << " other:\t";

		// store the last available status (the status may be omitted -> repeat last one)
		static uint8_t lastStatus = 0;

		// fetch the new status and check if this actually is a status (MSB set)
		// or if we have to use the last status (MSB not set)
		evt.status = *head;
		if (evt.status & 0x80) {++head; lastStatus = evt.status;} else {evt.status = lastStatus;}

		// check whether the event has 1 or 2 data-bytes
		if ( (evt.status&0xF0)>>4 == 12 || (evt.status&0xF0)>>4 == 13) {
			evt.d1 = *head++;
			evt.d2 = 0;
		} else {
			evt.d1 = *head++;
			evt.d2 = *head++;
		}

		// some fixes
		if (evt.getType() == MidiEventType::NOTE_ON && evt.d2 == 0) {evt.setType(MidiEventType::NOTE_OFF);}
		//std::cout << "status:" << (int)((evt.status>>4)-8) << "\tval:" << (int)evt.d1 << ":" << (int)evt.d2 << std::endl;

	}

	/** read a length-value with variable number of bytes (like UTF-8) */
	int readVarLen() {
		int len = 0;
		bool hasMore = false;
		do {
			hasMore = *head & 0x80;		// MSB = another byte following=
			int val = *head & 0x7F;		// all other bits = value;
			len <<= 7;
			len |= val;
			++head;
		} while (hasMore);
		return len;
	}



	/* the file to parse */
	K::File file;

	/** the destination to parse to */
	MidiFile& dst;


	/** the data buffer */
	uint8_t* buf;

	/** the current reading position */
	uint8_t* head;



	/** the length of the midi */
	unsigned int fileLenght;

	/** the number of tracks within the file */
	int numTracks;

	/** the type of the midi file (0, 1 is supported) */
	int midiType;

};

#include "MidiParser.h"


#endif /* MIDIPARSER_H_ */
