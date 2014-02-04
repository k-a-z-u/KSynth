/*
 * Generator.h
 *
 *  Created on: Nov 4, 2013
 *      Author: kazu
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <thread>
#include <vector>
#include <algorithm>

#include "misc/SoundSource.h"
#include "misc/DataTypes.h"
#include "misc/AudioFormat.h"

#include "Sequencer.h"
#include "output/SoundSink.h"
#include "SoundBaseBinder.h"



//static void bindCurrentThreadToCore(int coreNr) {

//#ifdef __linux__

//	// whole process
//	cpu_set_t my_set;
//	CPU_ZERO(&my_set);
//	CPU_SET(coreNr, &my_set);
//	//sched_setaffinity(0, sizeof(cpu_set_t), &my_set);

//	// one thread only
//	//unsigned long mask = 1;
//	int ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &my_set);
//	if (ret != 0) {std::cout << "error while settings thread affinity" << std::endl; exit(1);}

//#endif

//}


/** exception handling */
class GeneratorException : public std::exception {
public:
	GeneratorException(const std::string& msg) : msg(msg) {;}
	const char* what() const throw() override {return msg.c_str();}
private:
	std::string msg;
};


/** listen for events / errors within the generator */
class GeneratorListener {

public:

	/** an error occured */
	virtual void onError(GeneratorException& e) = 0;

};


/**
 * link between setup and audio-sink.
 * renders the samples using the requested sample
 * rate and triggers the sequencer.
 */
class Generator {

public:

	/** ctor */
	Generator(AudioFormat fmt) :
		fmt(fmt), src(nullptr), sink(nullptr), seq(nullptr), enabled(false) {;}

	/** dtor */
	~Generator() {
		;
	}

	/** set the sound source */
	void setSource(SoundBase* src) {
		this->src = src;
	}

	/** get the current sound source */
	SoundBase* getSource() const {
		return src;
	}


	/** set the sink to pipe the generated audio output to */
	void setSink(SoundSink* sink) {
		this->sink = sink;
	}

	/** get the currently selected SoundSink */
	SoundSink* getSink() const {
		return sink;
	}


	/** set the sequencer to use */
	void setSequencer(Sequencer* seq) {
		this->seq = seq;
	}

	/** get the underlying sequencer */
	Sequencer* getSequencer() const {
		return seq;
	}


	/** get the current audio format */
	AudioFormat getAudioFormat() const {
		return fmt;
	}


	/** add a new listener to the generator */
	void addListener(GeneratorListener* l) {
		listeners.push_back(l);
	}

	/** remove an existring listener from the generator */
	void removeListener(GeneratorListener* l) {
		auto match = [l] (const GeneratorListener* other) {return l == other;};
		listeners.erase( std::remove_if(listeners.begin(), listeners.end(), match), listeners.end() );
	}

	/** start rendering */
	void start() {

		// start only once
		if (enabled) {return;}

		// sanity checks
		if (src == nullptr) {throw GeneratorException("cannot start generator without attaching an audio-source first!");}

		// run
		//threadLock.lock(); {
			enabled = true;
			thread = std::thread(&Generator::render, this);
		//}; threadLock.unlock();

	}

	/** stop rendering */
	void stop() {

		// stop only once
		if (!enabled) {return;}
		enabled = false;

		thread.join();

	}

	/** is the generator currently acitve? (thread running) */
	bool isActive() {return enabled;}

	/** get the binder to handle connections between SoundBase devices */
	SoundBaseBinder& getBinder() {
		return binder;
	}


	/** render the song in a blocking mode until end of sequencer's track */
	void blockingRender() {

		enabled = true;
		SampleFrame frm = 0;

		// open the sound sink (might fail)
		sink->open(fmt);

		// inform the sequencer we are starting
		seq->start();

		// generate data and stream to sink (might fail)
		while(enabled) {

			frm += SNDBASE_BLK_SIZE;

			// inform the sequencer we are rendering the next block of frames
			seq->onGeneratorCallback(frm);

			// fetch data from all elements
			Amplitude* audio[2];
			audio[0] = get( src, 0, frm );
			audio[1] = get( src, 1, frm );

			// send output to the sink
			sink->push((const Amplitude**) audio, SNDBASE_BLK_SIZE);

			// check wether we are done
			if (seq->isDone()) {
				break;
			}

		}

		// cleanup
		seq->stop();
		sink->finalize();
		thread.detach();
		enabled = false;

	}



private:


	/**
		 * recursive function to fetch all outputs below this element.
		 * this will trigger a get() function for all attached (lower) devices.
		 * @param idx the channel we requested the output for
		 * @param out the pointer to attach the output to
		 */
	Amplitude* get(SoundBase* sb, unsigned int idx, unsigned int hash) {

		// lazy-allocate the output-buffer
		// TODO: better solution? one big buffer for all elements??
		sb->createOutputBuffer();
		sb->createInputPointers();

		// fill the buffer with zeros
		// TODO: maybe this can be avoided if the underlying device overwrites all data
		if (sb->hash != hash) {sb->resetOutputBuffer();}
		if (sb->hash != hash) {sb->resetInputPointers();}


		// synthesizer?
		if (sb->numInputs == 0) {

			// if this is a synthesizer -> create samples into output buffer
			if (hash != sb->hash) {sb->process(nullptr, sb->outputs);}

			sb->hash = hash;

		} else {

			// not a synthesizer
			// process data from lower levels

			if (sb->hash != hash) {

				// fetch data from all inputs
				// (next recursion depth)
				std::vector<Binding> bindings = binder.getBindingsToInputsOf(sb);
				for (Binding& b : bindings) {
					sb->inputs[b.dst.idx] = get( b.src.dev, b.src.idx, hash );
				}

				// process input data to output data
				sb->process(sb->inputs, sb->outputs);

			}

			sb->hash = hash;

		}

		// done.. attach to output and proceed with next layer
		// attach device's output to the requested output
		//std::cout << idx << ":" << outputs[idx] << std::endl;
		return sb->outputs[idx];

	}


	/** the thread */
	void render() {

		try {

			// perform magic
			blockingRender();

		} catch (std::exception& e) {

			// inform listeners
			GeneratorException ge( std::string("error during rendering:\n") + e.what() );
			for (GeneratorListener* l : listeners) {l->onError(ge);}
			seq->stop();
			enabled = false;

		}

	}


	/** the audio format to use */
	AudioFormat fmt;

	/** the source */
	SoundBase* src;

	/** the destination */
	SoundSink* sink;

	/** the sequencer to use */
	Sequencer* seq;

	/** the thread for rendering */
	std::thread thread;

	/** currently enabled or disabled? */
	bool enabled;

	/** handle connections between inputs and outputs of SoundBase devices */
	SoundBaseBinder binder;

	/** all registered listeners */
	std::vector<GeneratorListener*> listeners;

	//std::mutex threadLock;

};


#endif /* GENERATOR_H_ */
