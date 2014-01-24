#include "SoundSinks.h"

#include <KSynth/output/SoundSinkNull.h>
#include <KSynth/output/SoundSinkAlsa.h>
#include <KSynth/output/SoundSinkWaveOut.h>
#include <KSynth/output/SoundSinkExportWave.h>
#include <KSynth/output/SoundSinkExportMp3.h>



SoundSinks& SoundSinks::get() {
	static SoundSinks sinks;
	return sinks;
}


std::vector<SoundSink*> SoundSinks::getHardwareSinks() {

	// remove the unique pointer stuff for external handling
	std::vector<SoundSink*> vec;
	for ( auto& s : hardware ) {vec.push_back(s.get());}
	return vec;

}

SoundSinks::SoundSinks() {

	{
		std::unique_ptr<SoundSink> ptr(new SoundSinkNull());
		hardware.push_back( std::move(ptr) );
	}

#ifdef WITH_ALSA
	{
		std::vector< std::unique_ptr<SoundSinkAlsa> > devs;
		devs = SoundSinkAlsa::getAllDevices();
		for ( auto& dev : devs ) {
			hardware.push_back( std::move(dev) );
		}
	}
#endif

#ifdef WITH_COREAUDIO
	{
		;
	}
#endif

#ifdef WITH_WAVE_OUT
	{
		std::unique_ptr<SoundSink> ptr(new SoundSinkWaveOut());
		hardware.push_back( std::move(ptr) );
	}
#endif

#ifdef WITH_PULSE_AUDIO
	{
		;
	}
#endif


	{
		std::unique_ptr<SoundSink> ptr(new SoundSinkWave());
		exports.push_back( std::move(ptr) );
	}

#ifdef WITH_LAME
	{
		std::unique_ptr<SoundSink> ptr(new SoundSinkMp3());
		exports.push_back( std::move(ptr) );
	}
#endif

}
