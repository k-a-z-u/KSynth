#include "SoundSinks.h"

#include <KSynth/output/SoundSinkHardwareNull.h>
#include <KSynth/output/SoundSinkHardwareAlsa.h>
#include <KSynth/output/SoundSinkHardwareWaveOut.h>

#include <KSynth/output/SoundSinkExportWave.h>
#include <KSynth/output/SoundSinkExportMp3.h>



SoundSinks& SoundSinks::get() {
	static SoundSinks sinks;
	return sinks;
}


std::vector<SoundSinkHardware*> SoundSinks::getHardwareSinks() {

	// remove the unique pointer stuff for external handling
	std::vector<SoundSinkHardware*> vec;
	for ( auto& s : hardware ) {vec.push_back(s.get());}
	return vec;

}

std::vector<SoundSinkExport*> SoundSinks::getExportSinks() {

	// remove the unique pointer stuff for external handling
	std::vector<SoundSinkExport*> vec;
	for ( auto& s : exports ) {vec.push_back(s.get());}
	return vec;

}

SoundSinkHardware* SoundSinks::getHardwareSinkByName(const std::string& name) {
	for ( auto& s : hardware ) {if(s->getName() == name) {return s.get();}}
	return nullptr;
}

SoundSinkExport* SoundSinks::getExportSinkByName(const std::string &name) {
	for ( auto& s : exports ) {if(s->getName() == name) {return s.get();}}
	return nullptr;
}


SoundSinks::SoundSinks() {

	/** -------- hardware -------- */

	{
		std::unique_ptr<SoundSinkHardware> ptr(new SoundSinkHardwareNull());
		hardware.push_back( std::move(ptr) );
	}

#if defined(WITH_ALSA)
	{
		std::vector< std::unique_ptr<SoundSinkHardwareAlsa> > devs;
		devs = SoundSinkHardwareAlsa::getAllDevices();
		for ( auto& dev : devs ) {
			hardware.push_back( std::move(dev) );
		}
	}
#endif

#if defined(WITH_COREAUDIO)
	{
		;
	}
#endif

#if defined(WITH_WAVE_OUT)
	{
		std::unique_ptr<SoundSink> ptr(new SoundSinkWaveOut());
		hardware.push_back( std::move(ptr) );
	}
#endif

#if defined(WITH_PULSE_AUDIO)
	{
		;
	}
#endif


	/** -------- exports -------- */

	{
		std::unique_ptr<SoundSinkExport> ptr(new SoundSinkWave());
		exports.push_back( std::move(ptr) );
	}

#if defined(WITH_LAME)
	{
		std::unique_ptr<SoundSinkExport> ptr(new SoundSinkMp3());
		exports.push_back( std::move(ptr) );
	}
#endif

}
