/*
 * Sequencer.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kazu
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include <vector>
#include <exception>
#include <memory>

#include "midi/MidiFile.h"
#include "midi/rt/RtMidiWrapper.h"
#include "pattern/PatternSequencer.h"
#include "SequencerListener.h"
#include "SequencerTrack.h"



/** exception handling within sequencer */
class SequencerException : std::exception {
public:
	SequencerException(const std::string& msg) : msg(msg) {;}
	const char* what() const throw() {return msg.c_str();}
private:
	std::string msg;
};


/**
 * triggers note-devices with events like
 * note on/ off
 */
class Sequencer : public RtMidiEventListener {

public:

	/** ctor */
	Sequencer() : status(SequencerStatus::STOPPED), done(false) {
		setBeatsPerMinute(1^20);

		// MIDI TEST
		//		SequencerTrack track;
		//		tracks.push_back(track);
		//		midi.wrap = new RtMidiWrapper();
		//		if (!midi.wrap->getInputs().empty()) {
		//			midi.wrap->getInputs().at(0)->addListener(this);
		//		}

	}

	~Sequencer() {
		;
	}

	/** get all of the sequencer's tracks */
	const std::vector<std::unique_ptr<SequencerTrack>>& getTracks() {
		return tracks;
	}

	void onMidiEvent(RtMidiInDevice& src, MidiEvent evt) override {
		(void) src;
		midi.evts.push_back(evt);
	}

	/** bind a note-device to the given track number */
	void bind(unsigned int track, NoteDevice* nd) {
		if (tracks.size() <= track) {throw SequencerException("out of bounds while binding device to track");}
		tracks[track]->dev = nd;
		for (SequencerTrackListener* l : listeners.track) {l->onTracksChanged();}
	}

	/** unbind the given note-device from all tracks */
	void unbind(NoteDevice* nd) {
		for ( auto& st : tracks ) {
			if (st->dev == nd) {
				st->dev = nullptr;
			}
		}
		for (SequencerTrackListener* l : listeners.track) {l->onTracksChanged();}
	}

	/** add a new beat listener */
	void addBeatListener(SequencerBeatListener* l) {
		if (std::find(listeners.beat.begin(), listeners.beat.end(), l) != listeners.beat.end()) {
			throw SequencerException("the given beat-listener is already bound!");
		}
		listeners.beatLock.lock();
		listeners.beat.push_back(l);
		listeners.beatLock.unlock();
	}

	/** remove an existing beat listener */
	void removeBeatListener(SequencerBeatListener* l) {
		listeners.beatLock.lock();
		auto match = [l] (const SequencerBeatListener* other) {return other == l;};
		listeners.beat.erase( std::remove_if(listeners.beat.begin(), listeners.beat.end(), match), listeners.beat.end());
		listeners.beatLock.unlock();
	}

	/** add a new status listener */
	void addStatusListener(SequencerStatusListener* l) {
		if (std::find(listeners.status.begin(), listeners.status.end(), l) != listeners.status.end()) {
			throw SequencerException("the given status-listener is already bound!");
		}
		this->listeners.status.push_back(l);
		l->onStatusChange(status);
		l->onSettingsChange();
	}

	/** remove an existing status listener */
	void removeStatusListener(SequencerStatusListener* l) {
		auto match = [l] (const SequencerStatusListener* other) {return other == l;};
		listeners.status.erase( std::remove_if(listeners.status.begin(), listeners.status.end(), match), listeners.status.end());
	}


	/** add a new track listener */
	void addTrackListener(SequencerTrackListener* l) {
		if (std::find(listeners.track.begin(), listeners.track.end(), l) != listeners.track.end()) {
			throw SequencerException("the given track-listener is already bound!");
		}
		this->listeners.track.push_back(l);
	}

	/** remove an existing track listeners */
	void removeTrackListener(SequencerTrackListener *l) {
		auto match = [l] (const SequencerTrackListener* other) {return other == l;};
		listeners.track.erase( std::remove_if(listeners.track.begin(), listeners.track.end(), match), listeners.track.end());
	}


	/** add a new track to the sequencer */
	void addTrack(SequencerTrack* st) {
		std::unique_ptr<SequencerTrack> uniquePtr(st);
		tracks.push_back( std::move(uniquePtr) );
		for (SequencerTrackListener* l : listeners.track) {l->onTracksChanged();}
	}

	/** remove the given (existring) track */
	void removeTrack(const SequencerTrack& st) {
		auto match = [&st] ( std::unique_ptr<SequencerTrack>& other) {return &st == other.get();};
		tracks.erase(std::remove_if(tracks.begin(), tracks.end(), match), tracks.end());
		for (SequencerTrackListener* l : listeners.track) {l->onTracksChanged();}
	}

	/** get the song's length in multiples of 128th notes */
	TimeBase128 getSongLength() const {
		TimeBase128 len = 0;
		for (const auto& st : tracks) {
			if (st->getLength() > len) {len = st->getLength();}
		}
		return len;
	}

	/** attach the given midi to the sequencer */
	void import(MidiFile midi, int offset) {

		std::cout << "tick-div: " << midi.getTickDiv() << std::endl;
		float tickDiv = float(midi.getTickDiv()) / 128.0f;

		for (const MidiTrack& mt : midi.getTracks()) {

			// create new track
			std::unique_ptr<SequencerTrack> track(new SequencerTrack());

			// fetch name from midi name (if any)
			track->name = mt.getName();

			// some descriptive parts
			std::string desc = "Instruments: ";
			for (const std::string& s : mt.getInstrumentsAsString()) {desc += s + " ";}
			track->setDescription(desc);

			// convert all relative delays to absolute values
			int lastDelay = offset;
			for (const MidiEvent& evt : mt.getEvents()) {

				// convert delay from relative to absolute
				MidiEvent evtN = evt;
				evtN.delay = (unsigned int) ( float(lastDelay + evt.delay) / tickDiv );
				lastDelay += evt.delay;

				// append event to track
				track->events.add(evtN);

			}

			// attach
			tracks.push_back( std::move(track) );

		}

		/** inform listeners */
		for (SequencerTrackListener* l : listeners.track) {l->onTracksChanged();}

	}

	/** set the beats per minute to use */
	void setBeatsPerMinute(unsigned int bpm) {
		this->beatsPerMinute = bpm;
		for (SequencerStatusListener* l : listeners.status) {l->onSettingsChange();}
	}

	/** get current beats per minute configuration */
	unsigned int getBeatsPerMinute() const {
		return beatsPerMinute;
	}

	/** finished? */
	bool isDone() {
		return done;
	}

	/** remove all tracks from the sequencer */
	void clearTracks() {

		this->tracks.clear();

		/** inform listeners */
		for (SequencerTrackListener* l : listeners.track) {
			l->onTracksChanged();
		}

	}

	/** jump to the given time within the song */
	void jumpTo(TimeBase128 time) {

		// the jump-request is not executed directly
		// instead we set a marker which is checked within
		// the generator callback. thus we should be able to jump
		// without using mutex -> faster?
		timing.jumpTo = time;

	}




protected:

	friend class Generator;

	void fire(NoteDevice* dev, MidiEvent& evt) {

		if (!dev) {return;}

//		//std::cout << "track(" << track.trackNr << ")\t";
//		std::cout << "beat(" << evt.delay << ")\t";
//		std::cout << "cmd(" << (int) evt.getType() << ")\t";
//		std::cout << "d1(" << (int) evt.d1 << ")\t";
//		std::cout << "d2(" << (int) evt.d2 << ")\t";
//		std::cout << std::endl;

		if (evt.status) {
			if (evt.getType() == MidiEventType::NOTE_OFF) {
				//|| (track.evt.getType() == MidiEventType::NOTE_ON && track.evt.d2 == 0) ) {
				dev->stopNote( Note(evt.d1) );
				//std::cout << "stop" << std::endl;
			} else if (evt.getType() == MidiEventType::NOTE_ON) {
				dev->startNote( Note(evt.d1), evt.d2 / 127.0f);
				//std::cout << "start" << std::endl;
			}
		}

	}



	/** stop all currently pending notes within all attached devices */
	void stopAllNotes() {
		for ( auto& st : tracks) {
			st->curEventIdx = 0;
			if (st->dev) {st->dev->stopNotes();}
		}
	}

	void onBeat(TimeBase128 beat, Time time) {
		listeners.beatLock.lock();
		for (SequencerBeatListener* seq : listeners.beat) {
			seq->onBeat(beat, time);
		}
		listeners.beatLock.unlock();
	}

	/** reset the sequencer -> move to position 0 */
	void reset() {
		timing.cur128 = 0;
		timing.last128 = 0;
		timing.frm = 0;
		timing.time = 0;
		stopAllNotes();
	}

	/** called every X milliseconds from the generator */
	void onGeneratorCallback(SampleFrame frm) {

		// jump indication? -> jump
		// should be faster than using mutexes to prevent race conditions
		if (timing.jumpTo != -1) {jumpToIndication();}

		// delta (in sample frames) to last call
		SampleFrame deltaFrm = frm - timing.frm;
		timing.frm = frm;

		// delta (in seconds)
		// we need delta times to provide speed-changes during playback!
		Time deltaTime = Time(deltaFrm) / Time(48000);

		// absolute time
		timing.time = Time(frm) / Time(48000);

		// get current beat using beats-per-minute
		timing.cur128 += deltaTime * Time(beatsPerMinute) * 128.0f / 60.0f;

		// new 128th note?
		if ( (TimeBase128) timing.cur128 != (TimeBase128) timing.last128 ) {

			timing.last128 = timing.cur128;

			onBeat( (TimeBase128) timing.cur128, timing.time);

			// are we done yet?
			bool hasEvents = false;


			// process each track
			for ( auto& st : tracks ) {

				// skip empty tracks
				if (st->events.size() <= st->curEventIdx) {continue;}

				hasEvents = true;

				// execute all pending events
				while( timing.cur128 >= st->events[st->curEventIdx]->delay) {
					fire(st->dev, *st->events[st->curEventIdx]);
					++st->curEventIdx;
					if (st->events.size() <= st->curEventIdx) {break;}
				}

			}

			for (MidiEvent& e : midi.evts) {
				fire(tracks[0]->dev, e);
			}
			midi.evts.clear();

			// indicate wheter playback is done (no more events)
			this->done = !hasEvents;

		}

	}

	/** get the sequencer's current status (playing, stopped, ..) */
	SequencerStatus getStatus() const {
		return status;
	}

	/** start the sequencer */
	void start() {
		if (status == SequencerStatus::PLAYING) {return;}
		status = SequencerStatus::PLAYING;
		for (SequencerStatusListener* l : listeners.status) {l->onStatusChange(status);}
	}

	/** stop the sequencer */
	void stop() {
		if (status == SequencerStatus::STOPPED) {return;}
		reset();
		stopAllNotes();
		status = SequencerStatus::STOPPED;
		for (SequencerStatusListener* l : listeners.status) {l->onStatusChange(status);}
	}

	/** TODO start the sequencer in recording mode */
	void record() {
		if (status == SequencerStatus::RECORDING) {return;}
		status = SequencerStatus::RECORDING;
		for (SequencerStatusListener* l : listeners.status) {l->onStatusChange(status);}
	}

private:

	/** jump to the currently indicated time */
	void jumpToIndication() {
		stopAllNotes();
		timing.cur128 = (float) timing.jumpTo;
		timing.last128 = (float) timing.jumpTo;
		for ( auto& st : tracks ) {
			for (unsigned int i = 0; i < st->getEvents()->size(); ++i) {
				if ( st->getEvents()->at(i)->getDelay() >= TimeBase128(timing.jumpTo) ) {st->curEventIdx = i; break;}
			}
		}
		timing.jumpTo = -1;
	}

private:

	struct Timing {

		/** the current multiple of 128th notes. use float for accuracy */
		float cur128;

		/** the last multiple of 128th notes. use float for accuracy */
		float last128;

		/** time (in seconds) the playback is running */
		Time time;

		/** the last processed sample frame */
		SampleFrame frm;


		int jumpTo;

		/** ctor */
		Timing() : cur128(0), last128(0), time(0), frm(0), jumpTo(-1) {;}

	} timing;



	/** the beats per minute to use */
	Beat beatsPerMinute;

	/** the current status */
	SequencerStatus status;

	/** all tracks within the sequencer */
	std::vector< std::unique_ptr<SequencerTrack> > tracks;

	struct {
		RtMidiWrapper* wrap;
		std::vector<MidiEvent> evts;
	} midi;

	/** listeners */
	struct {
		std::vector<SequencerTrackListener*> track;
		std::vector<SequencerBeatListener*> beat;
		std::mutex beatLock;
		std::vector<SequencerStatusListener*> status;
	} listeners;

	bool done;

};


#endif /* SEQUENCER_H_ */
