/*
 * Generator.h
 *
 *  Created on: Nov 4, 2013
 *      Author: kazu
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <thread>
#include "misc/SoundSource.h"
#include "misc/DataTypes.h"
#include "misc/AudioFormat.h"
//#include "analyzer/FFTAnalyzer.h"

#include "Sequencer.h"
#include "output/SoundSink.h"
#include "SoundBaseBinder.h"

static void bindCurrentThreadToCore(int coreNr) {

#ifdef __linux__

	// whole process
	cpu_set_t my_set;
	CPU_ZERO(&my_set);
	CPU_SET(coreNr, &my_set);
	//sched_setaffinity(0, sizeof(cpu_set_t), &my_set);

	// one thread only
	//unsigned long mask = 1;
	int ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &my_set);
	if (ret != 0) {std::cout << "error while settings thread affinity" << std::endl; exit(1);}

#endif

}


/** exception handling */
class GeneratorException : public std::exception {
public:
	GeneratorException(const std::string& msg) : msg(msg) {;}
	const char* what() const throw() override {return msg.c_str();}
private:
	std::string msg;
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
		fmt(fmt), src(nullptr), sink(nullptr), seq(nullptr), thread(nullptr), enabled(false) {;}

	/** set the sound source */
	void setSource(SoundBase* src) {
		this->src = src;
	}

	/** set the sound destination */
	void setSink(SoundSink* sink) {
		this->sink = sink;
	}

	/** set the sequencer to use */
	void setSequencer(Sequencer* seq) {
		this->seq = seq;
	}

	/** start rendering */
	void start() {

		// start only once
		if (enabled) {return;}

		// sanity checks
		if (src == nullptr) {throw GeneratorException("cannot start generator without attaching an audio-source first!");}


		//seq->start();
		enabled = true;
		seq->start();
		thread = new std::thread(&Generator::render, this);

	}

	/** stop rendering */
	void stop() {

		// stop only once
		if (!enabled) {return;}
		enabled = false;
		thread->join();
		delete thread;
		thread = nullptr;
		seq->stop();
	}

	/** get the binder to handle connections between SoundBase devices */
	SoundBaseBinder& getBinder() {
		return binder;
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

		bindCurrentThreadToCore(0);

		//unsigned int sample = 0;
		//const unsigned int sample1MS = fmt.sampleRate / 200;		// 5 milliseconds

		SampleFrame frm = 0;

		try {

			sink->open();

			while(enabled) {

				frm += SNDBASE_BLK_SIZE;

				// throw sequencer event
				seq->onGeneratorCallback(frm);

				Amplitude* audio[2];
				audio[0] = get( src, 0, frm );
				audio[1] = get( src, 1, frm );

				try {
					sink->push((const Amplitude**) audio, SNDBASE_BLK_SIZE);
				} catch (std::exception& e) {
					std::cout << e.what() << std::endl;
					stop();
				}

			}

			sink->finalize();

		} catch (...) {
			std::cout << "EXCEPTION" << std::endl;
		}

		thread->detach();

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
	std::thread* thread;

	/** currently enabled or disabled? */
	bool enabled;

	/** handle connections between inputs and outputs of SoundBase devices */
	SoundBaseBinder binder;

	//FFTAnalyzer fft;

};


#endif /* GENERATOR_H_ */
