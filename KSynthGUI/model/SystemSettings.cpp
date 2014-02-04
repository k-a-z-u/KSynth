#include "SystemSettings.h"

#include "Context.h"
#include "SoundSinks.h"
#include "rack/Rack.h"

#include <KSynth/Generator.h>

#include "../lib/tinyxml2/tinyxml2.h"
using namespace tinyxml2;

SystemSettings::SystemSettings(Context& ctx) : ctx(ctx) {

	// start with a dummy (null) sink, to prevent crashes
	setSoundSink( SoundSinks::get().getHardwareSinks().at(0) );


}


void SystemSettings::setSoundSink(SoundSinkHardware* sink) {
	ctx.getGenerator()->setSink(sink);
}

SoundSinkHardware* SystemSettings::getSoundSink() const {
	return (SoundSinkHardware*) ctx.getGenerator()->getSink();
}


void SystemSettings::setGuiRefreshInterval(unsigned int ms) {
	ctx.getRack()->setRefreshInterval(ms);
}

unsigned int SystemSettings::getGuiRefreshInterval() const {
	return ctx.getRack()->getRefreshInterval();
}



std::string SystemSettings::load(const K::File& file) {

	XMLDocument doc;
	doc.LoadFile( file.getAbsolutePath().c_str() );

	XMLElement* nRoot = doc.FirstChildElement("KSynth");
	if (nRoot == nullptr) {return "";}

	// list of all encountered problems;
	std::string problems;

	XMLElement* nSettings = nRoot->FirstChildElement("Settings");
	if (!nSettings) {return "missing node for 'Settings'\n";}

	// sound-sink
	XMLElement* nSink = nSettings->FirstChildElement("SoundSink");
	if (nSink) {
		std::string name = nSink->Attribute("name");
		SoundSinkHardware* ss = SoundSinks::get().getHardwareSinkByName(name);
		if (ss) {
			setSoundSink(ss);
		} else {
			problems += "could not find a SoundSink named '"+name+"'\n";
		}
	} else {
		problems += "no 'SoundSink' given\n";
	}

	// refresh interval
	XMLElement* nGui = nSettings->FirstChildElement("GUI");
	if (nGui) {
		XMLElement* nGuiRefresh = nGui->FirstChildElement("Refresh");
		if (nGuiRefresh) {
			setGuiRefreshInterval(nGuiRefresh->IntAttribute("ms"));
		}
	}

	return problems;

}

void SystemSettings::save(const K::File& file) {

	XMLDocument doc;
	XMLElement* nRoot = doc.NewElement("KSynth");
	XMLElement* nSettings = doc.NewElement("Settings");

	doc.InsertFirstChild(nRoot);
	nRoot->InsertEndChild(nSettings);

	// the sound-sink
	XMLElement* nSink = doc.NewElement("SoundSink");
	nSink->SetAttribute("name", getSoundSink()->getName().c_str());
	nSettings->InsertEndChild(nSink);

	// refresh
	XMLElement* nGui = doc.NewElement("GUI");
	nSettings->InsertEndChild(nGui);
	XMLElement* nGuiRefresh = doc.NewElement("Refresh");
	nGuiRefresh->SetAttribute("ms", getGuiRefreshInterval());
	nGui->InsertEndChild(nGuiRefresh);



	doc.SaveFile( file.getAbsolutePath().c_str() );

}
