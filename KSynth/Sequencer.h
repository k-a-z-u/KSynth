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
	Sequencer() : curBeat(0), frm(0), status(SequencerStatus::STOPPED), done(false) {
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
//		delete midi.wrap;
	}

	/** get all of the sequencer's tracks */
	std::vector<SequencerTrack>* getTracks() {
		return &tracks;
	}

	void onMidiEvent(RtMidiInDevice& src, MidiEvent evt) override {
		(void) src;
		midi.evts.push_back(evt);
	}

	/** bind a note-device to the given track number */
	void bind(unsigned int track, NoteDevice* nd) {
		if (tracks.size() <= track) {throw SequencerException("out of bounds while binding device to track");}
		tracks[track].dev = nd;
		for (SequencerTrackListener* l : listeners.track) {l->onTracksChanged();}
	}

	/** unbind the given note-device from all tracks */
	void unbind(NoteDevice* nd) {
		for (SequencerTrack& st : tracks) {
			if (st.dev == nd) {
				st.dev = nullptr;
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
	void addTrack(SequencerTrack st) {
		tracks.push_back(st);
		for (SequencerTrackListener* l : listeners.track) {l->onTracksChanged();}
	}

	/** attach the given midi to the sequencer */
	void import(MidiFile midi, int offset) {

		std::cout << "SPEED: " << midi.getSpeed() << std::endl;
		float speed = float(midi.getSpeed()) / 128.0f; //

		for (const MidiTrack& mt : midi.getTracks()) {

			// create new track
			SequencerTrack track;

			// fetch name from midi name (if any)
			track.name = mt.getName();

			// some descriptive parts
			std::string desc = "Instruments: ";
			for (const std::string& s : mt.getInstrumentsAsString()) {desc += s + " ";}
			track.setDescription(desc);

			// convert all relative delays to absolute values
			unsigned int lastDelay = offset;
			for (const MidiEvent& evt : mt.getEvents()) {

				// convert delay from relative to absolute
				MidiEvent evtN = evt;
				evtN.delay = (unsigned int) ( float(lastDelay + evt.delay) / speed );
				lastDelay += evt.delay;

				// append event to track
				track.events.add(evtN);

			}

			// attach
			tracks.push_back(track);

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

protected:

	friend class Generator;

	void fire(NoteDevice* dev, MidiEvent& evt) {

		if (!dev) {return;}

		//std::cout << "track(" << track.trackNr << ")\t";
		std::cout << "beat(" << evt.delay << ")\t";
		std::cout << "cmd(" << (int) evt.getType() << ")\t";
		std::cout << "d1(" << (int) evt.d1 << ")\t";
		std::cout << "d2(" << (int) evt.d2 << ")\t";
		std::cout << std::endl;

		if (evt.status) {
			if (evt.getType() == MidiEventType::NOTE_OFF) {
				//|| (track.evt.getType() == MidiEventType::NOTE_ON && track.evt.d2 == 0) ) {
				dev->stopNote( Note(evt.d1) );
				std::cout << "stop" << std::endl;
			} else if (evt.getType() == MidiEventType::NOTE_ON) {
				dev->startNote( Note(evt.d1), evt.d2 / 127.0f);
				std::cout << "start" << std::endl;
			}
		}

	}


	void onBeat(Beat beat, Time time) {
		listeners.beatLock.lock();
		for (SequencerBeatListener* seq : listeners.beat) {
			seq->onBeat(beat, time);
		}
		listeners.beatLock.unlock();
	}

	void restart() {
		this->curBeat = 0;
		this->frm = 0;
		this->time = 0;
		for (SequencerTrack& st : tracks) {
			st.curEventIdx = 0;
			if (st.dev) {st.dev->stopNotes();}
		}
	}

	/** called every X milliseconds from the generator */
	void onGeneratorCallback(SampleFrame frm) {

		// delta to last call
		SampleFrame deltaFrm = frm - this->frm;
		this->frm = frm;
		Time deltaTime = Time(deltaFrm) / Time(48000);

		// absolute time
		this->time = Time(frm) / Time(48000);

		// get current beat using beats-per-minute
		curBeat += deltaTime * Time(beatsPerMinute) / 60.0f;

		// convert to current 128th note
		Beat th128 = (int) (curBeat * (128));
		static Beat lastTH128 = 0;

		//std::cout << "#" << beat << std::endl;

		if (lastTH128 != th128) {
			lastTH128 = th128;

			onBeat(lastTH128, time);

			// are we done yet?
			bool done = true;

			// process each track
			for (SequencerTrack& st : tracks) {

				// skip empty tracks
				if (st.events.size() <= st.curEventIdx) {continue;}

				done = false;

				// execute all pending events
				while(th128 >= st.events[st.curEventIdx]->delay) {
					fire(st.dev, *st.events[st.curEventIdx]);
					++st.curEventIdx;
					if (st.events.size() <= st.curEventIdx) {break;}
				}

			}

			for (MidiEvent& e : midi.evts) {
				fire(tracks[0].dev, e);
			}
			midi.evts.clear();

			this->done = done;

		}

	}

	/** get the sequencer's current status (playing, stopped, ..) */
	SequencerStatus getStatus() const {
		return status;
	}

	/** start the sequencer */
	void start() {
		status = SequencerStatus::PLAYING;
		for (SequencerStatusListener* l : listeners.status) {l->onStatusChange(status);}
	}

	/** stop the sequencer */
	void stop() {
		status = SequencerStatus::STOPPED;
		for (SequencerStatusListener* l : listeners.status) {l->onStatusChange(status);}
	}

	/** TODO start the sequencer in recording mode */
	void record() {
		status = SequencerStatus::RECORDING;
		for (SequencerStatusListener* l : listeners.status) {l->onStatusChange(status);}
	}

private:

	/** the current beat */
	float curBeat;

	/** the beats per minute to use */
	Beat beatsPerMinute;

	Time time;
	SampleFrame frm;

	/** the current status */
	SequencerStatus status;

	/** all tracks within the sequencer */
	std::vector<SequencerTrack> tracks;

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
