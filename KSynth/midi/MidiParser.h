/*
 * MidiParser.h
 *
 *  Created on: Nov 1, 2013
 *      Author: kazu
 */

#ifndef MIDIPARSER_H_
#define MIDIPARSER_H_

#include <KLib/fs/File.h>
#include <KLib/streams/Endian.h>

class MidiParserException : public std::exception {
public:
	MidiParserException(const std::string& str, const K::File& file) : str() {
		this->str = "error:\t" + str + "\nfile:\t" + file.getAbsolutePath();
	}
	MidiParserException(const std::string& str) : str(str) {;}
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
		file(f), dst(dst), numTracks(0) {
		read();
		parse();
	}

	/** dtor */
	~MidiParser() {

	}


private:

	/** read the file completely */
	void read() {

		// open
		FILE *fh = fopen(file.getAbsolutePath().c_str(), "rb");
		if (fh == nullptr) {throw MidiParserException("could not open midi file", file);}

		// seek to the file's end to get the file's length
		fseek(fh, 0, SEEK_END);
		unsigned int length = (unsigned int) ftell(fh);
		rewind(fh);
		data.alloc(length);

		// read file
		size_t res = fread(data.buffer, data.length, 1, fh);
		if (res != 1) {throw MidiParserException("error while reading midi file", file);}

		// cleanup
		fclose(fh);

	}

	/** parse the midi file */
	void parse() {

		// check signature
		if (memcmp(data.buffer+0, "MThd", 4)) {throw MidiParserException("this is not a midi file", file);}

		// read all chunks within the midi-file until EOF
		do {
			readChunk();
		} while ( !data.isEOF() );

		// if the read file was midi V0 (one track only)
		// we create one track per channel as this is more convenient
		if (dst.version == MidiVersion::SINGLE_TRACK) {
			splitTracks();
		}

	}

	/** read the next chunk within the file */
	void readChunk() {

		// type of the chunk ("MTrk" or "MThd")
		std::string type((const char*)(data.head), 4);							data.head += 4;

		// length of the chunk
		uint32_t length = data.readInt();

		uint8_t* oldHead = data.head;

		// what to do
		if		(type == "MThd")	{readHeader(length);}
		else if	(type == "MTrk")	{readTrack(length);}
		else						{throw MidiParserException("found unknown chunk type: '" + type + "'", file);}

		// done
		// this will ensure reading properly even when reading
		// a chunk fails to read
		data.head = oldHead + length;

	}

	/** parse the midi header */
	void readHeader(unsigned int length) {

		std::cout << "header" << std::endl;
		if (length != 6) {throw MidiParserException("header length != 6 is currently not supported", file);}

		// midi type (currently only 0/1 are supported)
		int version = data.readShort();
		if (version > 1) {throw MidiParserException("midi-type != 0/1 is currently not supported", file);}
		dst.version = (MidiVersion) version;

		// number of tracks (version 0 must have only 1 track!)
		numTracks = data.readShort();
		if (version == 0 && numTracks != 1) {throw MidiParserException("midi-type is 0 but file contains more than one channel!", file);}

		// delta time unit (negative value = bit15 set = SMPTE -> currently not supported)
		int16_t unit = data.readShort();
		if (unit < 0) {throw MidiParserException("negative delta times are currently not supported", file);}
		dst.tickDiv = unit;

	}

	//	/** parse data for one track */
	//	void readTrack(unsigned int length) {

	//		readTrack(lenght);
	//		//if (dst.version == MidiVersion.SINGLE_TRACK)	{readTrackV0(length);}
	//		//else											{readTrackV1(length);}
	//	}


	//	/**
	//	 * midi V0 contains only ONE track.
	//	 * for better editing we will create one track per channel
	//	 * and split events into multiple tracks (max 16)
	//	 */
	//	void readTrackV0(unsigned int length) {

	//		// max 16 tracks are allowed (equals number of midi channels)
	//		MidiTrack track[16];

	//		// check the length of the track data
	//		uint8_t* start = head;

	//		// temporals
	//		MidiEvent evt;
	//		unsigned int cnt = 0;

	//		do {

	//			// the delay of the event
	//			evt.delay = readVarLen();

	//			// peek into the even type to distinguish
	//			// what to do next
	//			evt.status = *head;

	//			if		( (evt.status&0xFF) == 0xFF)	{++head; readTrackMeta(track[0]);}
	//			else if	( (evt.status&0xF0) == 0xF)		{++head; readTrackSystem();}
	//			else									{
	//				readTrackOther(evt);
	//				unsigned int chan = evt.getChannel();
	//				track[chan].add(evt);
	//			}

	//			++cnt;

	//		} while ((head-start) < length);

	//		// append tracks to file (skip empty ones
	//		for (unsigned int i = 0; i < 16; ++i) {
	//			if (track[i].isEmpty()) {continue;}
	//			dst.add(track[i]);
	//		}

	//	}

	/** parse data for one track */
	void readTrack(unsigned int length) {

		std::cout << "track" << std::endl;
		MidiTrack track;

		// check the length of the track data
		uint8_t* start = data.head;

		// temporals
		MidiEvent evt;
		unsigned int cnt = 0;

		do {

			// the delay of the event
			evt.delay = data.readVarLen();

			// peek into the event type to distinguish
			// what to do next
			evt.status = data.peekByte();

			if		( (evt.status&0xFF) == 0xFF)	{readTrackMeta(track);}
			else if	( (evt.status&0xF0) == 0xF)		{readTrackSystem();}
			else									{readTrackOther(evt); track.add(evt);}

			++cnt;

		} while ((data.head-start) < length);

		// append track to file
		dst.add(track);

	}

	void readTrackMeta(MidiTrack& trk) {

		std::cout << " meta:\t";

		data.readByte();						// the 0xFF for "this is meta"
		uint8_t type = data.readByte();			// the byte indicating the type of meta information
		uint32_t len = data.readVarLen();		// the length for the payload of meta information

		switch (type) {

			case 0x02:
				std::cout << "copyright notice" << std::endl;
				break;

			case 0x03:
				std::cout << "track name" << std::endl;
				trk.name = std::string( (const char*)(data.head), len );
				break;

			case 0x04:
				std::cout << "instrument name" << std::endl;
				break;

			case 0x05:
				std::cout << "lyric" << std::endl;
				break;

			case 0x06:
				std::cout << "marker" << std::endl;
				break;

			case 0x07:
				std::cout << "cue point" << std::endl;
				break;

			case 0x51:
				std::cout << len << std::endl;
				std::cout << "Tempo" << std::endl;
				break;

			case 0x54:
				std::cout << "SMPTE offset" << std::endl;
				break;

			case 0x58:
				std::cout << "Time signature" << std::endl;
				break;

			case 0x59:
				std::cout << "Key signature" << std::endl;
				break;

			case 0x2F:
				std::cout << "end of track" << std::endl;
				break;

			default:
				std::cout << "(" << (int)type << ")\t";
				std::string content((const char*)(data.head), len);
				std::cout << '"' << content << '"' << std::endl;
				break;

		}

		// set head to the next event and proceed
		data.head += len;

	}

	// TODO!
	void readTrackSystem() {
		std::cout << " system:\t";
		uint8_t type = data.readByte();
		unsigned int len = data.readVarLen();
		std::cout << "len:" << len << std::endl;
		data.head += len;
	}

	/** parse normal track events and store them within the given data structure */
	void readTrackOther(MidiEvent& evt) {

		//std::cout << " other:\t";

		// store the last available status (the status may be omitted -> repeat last one)
		static uint8_t lastStatus = 0;

		// fetch the new status and check if this actually is a status (MSB set)
		// or if we have to use the last status (MSB not set)
		evt.status = *data.head;
		if (evt.status & 0x80) {++data.head; lastStatus = evt.status;} else {evt.status = lastStatus;}

		// check whether the event has 1 or 2 data-bytes
		if ( (evt.status&0xF0)>>4 == 12 || (evt.status&0xF0)>>4 == 13) {
			evt.d1 = data.readByte();
			evt.d2 = 0;
		} else {
			evt.d1 = data.readByte();
			evt.d2 = data.readByte();
		}

		// some fixes
		if (evt.getType() == MidiEventType::NOTE_ON && evt.d2 == 0) {evt.setType(MidiEventType::NOTE_OFF);}
		//std::cout << "status:" << (int)((evt.status>>4)-8) << "\tval:" << (int)evt.d1 << ":" << (int)evt.d2 << std::endl;

	}

	/** for midi V0: split the one and only track by channel */
	void splitTracks() {

		std::cout << "splitting midi v0 tracks" << std::endl;

		// max 16 tracks are allowed (equals number of midi channels)
		MidiTrack track[16];

		// store the current delta for each track
		uint32_t deltas[16] = {0};

		// process each midi event within the one-and-only track of midi v0
		for ( const MidiEvent& e : dst.getTrack(0).getEvents() ) {

			// increment all deltas
			for (unsigned int i = 0; i < 16; ++i) {deltas[i] += e.delay;}

			// attach event to the track behind its channel number
			unsigned int idx = e.getChannel();
			MidiEvent cpy = e;
			cpy.delay = deltas[idx];
			deltas[idx] = 0;
			track[idx].add(cpy);

		}

		// append tracks to file (skip empty ones
		dst.deleteTracks();
		for (unsigned int i = 0; i < 16; ++i) {
			if (track[i].isEmpty()) {continue;}
			dst.add(track[i]);
		}


	}


	/* the file to parse */
	K::File file;

	/** the destination to parse to */
	MidiFile& dst;



	struct MidiData {

		/** holds the binary midi data */
		uint8_t* buffer;

		/** the current reading position */
		uint8_t* head;

		/** the length of the midi data */
		unsigned int length;

		/** ctor */
		MidiData() : buffer(nullptr), head(nullptr), length(0) {
			;
		}

		/** ~dtor */
		~MidiData() {
			std::cout << "midi buffer cleanup" << std::endl;
			if (buffer) {free(buffer); buffer = nullptr;}
		}

		/** allocate the buffer to hold the given size */
		void alloc(unsigned int length) {
			this->length = length;
			if (buffer) {free(buffer); buffer = nullptr;}
			buffer = (uint8_t*) malloc(length);
			if (buffer == nullptr) {throw MidiParserException("out of memory");}
			head = buffer;
		}

		/** is the head at EOF? */
		bool isEOF() {return (head-buffer) >= length;}

		/** read one byte. DO NOT increment the head */
		uint8_t peekByte() {return *head;}

		/** read one byte. increments the head */
		uint8_t readByte() {return *head++;}

		/** read two bytes. increments the head */
		uint16_t readShort() {
			uint16_t val = K::Endian::bigToHost( *((uint16_t*)(head)) );
			head += 2;
			return val;
		}

		/** read four bytes. increments the head */
		uint32_t readInt() {
			uint32_t val = K::Endian::bigToHost( *((uint32_t*)(head)) );
			head += 4;
			return val;
		}

		/** read a length-value with variable number of bytes (like UTF-8) */
		unsigned int readVarLen() {
			unsigned int len = 0;
			bool hasMore = false;
			do {
				uint8_t byte = readByte();
				hasMore = byte & 0x80;			// MSB = another byte following=
				len <<= 7;						// shift previously read data up by 7 bits
				len |= byte & 0x7F;				// append all bits but the highest one
			} while (hasMore);
			return len;
		}

	} data;


	/** the number of tracks within the file */
	int numTracks;

};

#include "MidiParser.h"


#endif /* MIDIPARSER_H_ */
