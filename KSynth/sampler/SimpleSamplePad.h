#ifndef SIMPLESAMPLEPAD_H
#define SIMPLESAMPLEPAD_H

#include "../misc/SoundSource.h"
#include "SampleFactory.h"

#include <KLib/fs/File.h>
#include <vector>

/** the number of loadable samples */
#define SSP_SAMPLES		16

/**
 * represents one loaded (and playable) sample with
 * some additional parameters the user may modify.
 */
struct SimpleSamplePadSample {

	/** the sample to play */
	Sample sample;

	/** the user-selected volume to apply to the sample */
	Volume vol;

	/** filename (relative to the working directory) */
	std::string file;

	SimpleSamplePadSample() : vol(1.0f) {;}

};


/**
 * all currently active samples
 */
struct SimpleSamplePadActive {

	/** the sample (and it's settings) to play */
	const SimpleSamplePadSample* ssps;

	/** the volume to use for playing this sample (e.g. thrength of key-press) */
	Volume vol;

	/** the current position within the sample */
	SampleFrame pos;

	/** ctor */
	SimpleSamplePadActive(const SimpleSamplePadSample* ssps, Volume vol) : ssps(ssps), vol(vol), pos(0) {;}

};



/**
 * just play loaded samples when notes are triggered.
 * can be e.g. used in combination with a Launchpad
 * from Novation.
 *
 */
class SimpleSamplePad : public SoundSource {

public:

	/** ctor */
	SimpleSamplePad() : SoundSource(2) {

	}

	void process(Amplitude** inputs, Amplitude** outputs) override {

		(void) inputs;

		for (unsigned int j = 0; j < active.size(); ++j) {

			SimpleSamplePadActive sspa = active[j];

			// attach all samples to the output buffer
			for (unsigned int i = 0; i < getSamplesPerProcess(); ++i) {
				outputs[0][i] += sspa.ssps->sample.get(sspa.pos).left * sspa.vol;
				outputs[1][i] += sspa.ssps->sample.get(sspa.pos).right * sspa.vol;
				++sspa.pos;
			}

			// sample done? -> remove
			if (active[j].pos >= sspa.ssps->sample.getNumFrames()) {
				active.erase( active.begin() + j ); --j;
			}

		}

	}


	/** get the amount of used memory (in bytes) */
	unsigned int getMemoryUsage() {
		unsigned int ret = 0;
		for (SimpleSamplePadSample& ssps : samples) {
			ret += ssps.sample.getMemoryUsage();
		}
		return ret;
	}

	void playSample(unsigned int idx, Volume vol) {
		if (idx >= SSP_SAMPLES) {return;}
		SimpleSamplePadActive sspa( &samples[idx], vol );
		active.push_back( sspa );
	}

	std::string getProductString() const override {
		return "SimpleSamplePad";
	}

	unsigned int getNumParameters() const override {
		return 0;
	}

protected:

	/** load sample using the given filename into the provided idx */
	void loadSample(const std::string& file, unsigned int idx) {
		K::File f(file);
		loadSample(f, idx);
	}

	/** load sample using the given filename into the provided idx */
	void loadSample(const K::File& file, unsigned int idx) {
		if (idx >= SSP_SAMPLES) {return;}
		samples[idx].sample = SampleFactory::load(file, getSampleRate());
		samples[idx].file = file.getRelativePath();
	}




private:

	/** all loaded samples */
	SimpleSamplePadSample samples[SSP_SAMPLES];

	/** all currently active (playing) samples */
	std::vector<SimpleSamplePadActive> active;


};

#endif // SIMPLESAMPLEPAD_H
