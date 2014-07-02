#include "RackFactory.h"

#include "rack/Synth1.h"
#include "rack/Mixer1.h"
#include "rack/DrumComputer1.h"
#include "rack/Sequencer1.h"
#include "rack/MasterTarget1.h"
#include "rack/SamplePad1.h"
#include "rack/Reverb1.h"
#include "rack/Phaser1.h"

RackFactory::RackFactory(Context& ctx) : ctx(ctx) {
	;
}

RackFactory::~RackFactory() {
	;
}

std::vector<std::string> RackFactory::getDevices() const {

	std::vector<std::string> ret;
	ret.push_back("SimpleSynth");
	ret.push_back("SimpleDrumComputer");
	ret.push_back("SimpleMixer");
	ret.push_back("MasterTarget");
	ret.push_back("SimpleSamplePad");
	ret.push_back("AllPassReverb");
	ret.push_back("Phaser");
	return ret;

}

RackElement* RackFactory::getByString(const std::string& str, Context& ctx) const {

	if		(str == "SimpleSynth")			{return new Synth1(ctx);}
	else if	(str == "SimpleDrumComputer")	{return new DrumComputer1(ctx);}
	else if	(str == "SimpleMixer")			{return new Mixer1(ctx);}
	else if	(str == "MasterTarget")			{return new MasterTarget1(ctx);}
	else if	(str == "SimpleSamplePad")		{return new SamplePad1(ctx);}
	else if	(str == "AllPassReverb")		{return new Reverb1(ctx);}
	else if	(str == "Phaser")				{return new Phaser1(ctx);}
	throw RackFactoryException("could not find a device named '" + str + "'!");

}
