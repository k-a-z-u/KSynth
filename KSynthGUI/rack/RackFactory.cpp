#include "RackFactory.h"

#include "rack/Synth1.h"
#include "rack/Mixer1.h"
#include "rack/DrumComputer1.h"
#include "rack/Sequencer1.h"
#include "rack/MasterTarget1.h"


RackFactory::RackFactory(Context& ctx) : ctx(ctx) {

//	// create all available elements
//	Synth1* synth = new Synth1(0);
//	elems[synth->getProductString()] = std::unique_ptr<RackElement>(synth);

//	Mixer1* mixer = new Mixer1(0);
//	elems[mixer->getProductString()] = std::unique_ptr<RackElement>(mixer);

//	DrumComputer1* drumComputer = new DrumComputer1(0);
//	elems[drumComputer->getProductString()] = std::unique_ptr<RackElement>(drumComputer);

//	Sequencer1* sequencer = new Sequencer1(0);
//	elems[sequencer->getProductString()] = std::unique_ptr<RackElement>(sequencer);

}


RackFactory::~RackFactory() {
//	this->elems.clear();
}

std::vector<std::string> RackFactory::getDevices() const {
//	std::vector<std::string> keys;
//	keys.reserve(elems.size());
//	for(auto& kv : elems) {
//		keys.push_back(kv.first);
//	}
//	return keys;
	std::vector<std::string> ret;
	ret.push_back("SimpleSynth");
	ret.push_back("SimpleDrumComputer");
	ret.push_back("SimpleMixer");
	ret.push_back("MasterTarget");
	return ret;
}

RackElement* RackFactory::getByString(const std::string& str, Context& ctx) const {

//	if (elems.find(str) == elems.end()) {
//		throw "could not find a device named '" + str + "'!";
//	}
//	return elems.at(str)->createFromPrototype(ctx);
	if		(str == "SimpleSynth")			{return new Synth1(ctx);}
	else if	(str == "SimpleDrumComputer")	{return new DrumComputer1(ctx);}
	else if	(str == "SimpleMixer")			{return new Mixer1(ctx);}
	else if	(str == "MasterTarget")			{return new MasterTarget1(ctx);}
	throw RackFactoryException("could not find a device named '" + str + "'!");

}
