#ifndef SOUNDSINKS_H
#define SOUNDSINKS_H

#include <memory>
#include <vector>
#include <KSynth/output/SoundSink.h>


/**
 * this class provides a list of all available
 * SoundSinks on the target machine.
 *
 * this list depends on both, the compiled sinks
 * and the installed hardware on the target matchine.
 *
 * all available sinks are handled internally and are
 * deleted when this class goes out of scope.
 *
 *
 */
class SoundSinks {

public:

	/** singleton access */
	static SoundSinks& get();

	/**
	 * get a list of available hardware sinks.
	 * DO NOT DELETE ANY OF THOSE POINTERS
	 */
	std::vector<SoundSink*> getHardwareSinks();


private:

	/** hidden ctor */
	SoundSinks();
	SoundSinks(const SoundSinks&);
	SoundSinks& operator = (const SoundSinks&);

	/** all available hardware sound sinks */
	std::vector< std::unique_ptr<SoundSink> > hardware;

	/** all available export sound sinks */
	std::vector< std::unique_ptr<SoundSink> > exports;

};

#endif // SOUNDSINKS_H
