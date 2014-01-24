#include "SystemSettings.h"

#include "Context.h"
#include "SoundSinks.h"
#include "rack/Rack.h"

#include <KSynth/Generator.h>

SystemSettings::SystemSettings(Context& ctx) : ctx(ctx) {

	// start with a dummy (null) sink, to prevent crashes
	setSoundSink( SoundSinks::get().getHardwareSinks().at(0) );


}


void SystemSettings::setSoundSink(SoundSink* sink) {
	ctx.getGenerator()->setSink(sink);
}

SoundSink* SystemSettings::getSoundSink() const {
	return ctx.getGenerator()->getSink();
}


void SystemSettings::setGuiRefreshInterval(unsigned int ms) {
	ctx.getRack()->setRefreshInterval(ms);
}

unsigned int SystemSettings::getGuiRefreshInterval() const {
	return ctx.getRack()->getRefreshInterval();
}
