#include "WorkspaceLoader.h"
#include "../model/Context.h"
#include "../rack/RackFactory.h"
#include "../rack/Rack.h"

#include <QMessageBox>

#include <KSynth/misc/SoundBase.h>
#include <KSynth/Generator.h>


WorkspaceLoader::WorkspaceLoader(Context& ctx) : ctx(ctx) {
	;
}

WorkspaceLoader::~WorkspaceLoader() {
	;
}

void WorkspaceLoader::load(const K::File& file) {

	ctx.getRack()->reset();
	ctx.getSequencer()->clearTracks();

	// open
	XMLDocument doc;
	bool ok = doc.LoadFile(file.getAbsolutePath().c_str());
	//if (!ok) {throw "error while loading file!";}

	// root
	XMLElement* nRoot = doc.RootElement();

	// sub-elements
	XMLElement* nSettings = nRoot->FirstChildElement("Settings");
	XMLElement* nRack = nRoot->FirstChildElement("Rack");
	XMLElement* nBindings = nRoot->FirstChildElement("Bindings");
	XMLElement* nTracks = nRoot->FirstChildElement("Tracks");

	// sanity check
	if (!nRack)		{throw WorkspaceLoaderException("the file is missing an element for 'Rack'");}
	if (!nBindings)	{throw WorkspaceLoaderException("the file is missing an element for 'Bindings'");}
	if (!nTracks)	{throw WorkspaceLoaderException("the file is missing an element for 'Tracks'");}

	// load workspace settings
	loadSettings(nSettings);

	// load all rack elements
	loadRackElements(nRack);

	// load all bindings
	loadBindings(nBindings);

	// load all tracks
	loadTracks(nTracks);

}


void WorkspaceLoader::loadSettings(XMLElement* nSettings) {

	XMLElement* nSeq = nSettings->FirstChildElement("Sequencer");
	if (!nSeq) {throw WorkspaceLoaderException("the Settings node is missing a child for Sequencer");}

	// sequencer settings
	unsigned int bpm = nSeq->IntAttribute("bpm");
	ctx.getSequencer()->setBeatsPerMinute(bpm);

}


void WorkspaceLoader::loadRackElements(XMLElement* nRack) {

	// load all rack elements
	for (
		 XMLElement* nRackElement = nRack->FirstChildElement("RackElement");
		 nRackElement;
		 nRackElement = nRackElement->NextSiblingElement("RackElement")
		 ) {

		// details
		unsigned int id = nRackElement->IntAttribute("id");
		std::string type = nRackElement->Attribute("type");
		const char* userName = nRackElement->Attribute("user_name");

		// create
		RackElement* re;
		try {
			re = ctx.getRackFactory()->getByString(type, ctx);
			if (userName) {re->setUserName(userName);}
		} catch (RackFactoryException& e) {
			QMessageBox::critical(nullptr, "error loading file", e.what());
			return;
		}

		ctx.getRack()->add(re);

		// add to ID-list
		idToElement[id] = re;

		// params-root
		XMLElement* nParams = nRackElement->FirstChildElement("Parameters");

		// load all parameters
		SoundBase* sb = dynamic_cast<SoundBase*>(re);
		if (!sb) {continue;}

		for (
			 XMLElement* nParam = nParams->FirstChildElement("Parameter");
			 nParam;
			 nParam = nParam->NextSiblingElement("Parameter")
			 ) {

			unsigned int idx = nParam->IntAttribute("num");
			std::string name = nParam->Attribute("name");
			float value = nParam->FloatAttribute("value");

			sb->setParameter(idx, value);
			sb->setParameterName(idx, name);

		}
	}

}


void WorkspaceLoader::loadBindings(XMLElement* nBindings) {

	// load all rack elements
	for (
		 XMLElement* nBinding = nBindings->FirstChildElement("Binding");
		 nBinding;
		 nBinding = nBinding->NextSiblingElement("Binding")
		 ) {

		unsigned int srcID = nBinding->IntAttribute("src_id");
		unsigned int dstID = nBinding->IntAttribute("dst_id");
		unsigned int srcPort = nBinding->IntAttribute("src_port");
		unsigned int dstPort = nBinding->IntAttribute("dst_port");
		RackElement* reSrc = idToElement[srcID];
		RackElement* reDst = idToElement[dstID];
		SoundBase* sbSrc = dynamic_cast<SoundBase*>(reSrc);
		SoundBase* sbDst = dynamic_cast<SoundBase*>(reDst);

		if (sbSrc == nullptr || sbDst == nullptr) {
			throw "error while loading workspace. illegal binding!";
		}

		ctx.getGenerator()->getBinder().addBinding(srcPort, sbSrc, dstPort, sbDst);

	}

}


void WorkspaceLoader::loadTracks(XMLElement* nTracks) {

	// load all tracks elements
	for (
		 XMLElement* nTrack = nTracks->FirstChildElement("Track");
		 nTrack;
		 nTrack = nTrack->NextSiblingElement("Track")
		 ) {

		// track details
		std::string name = nTrack->Attribute("name");
		std::string desc = nTrack->FirstChildElement("Desc")->GetText();

		// create track
		SequencerTrack st;
		st.setName(name);
		st.setDescription(desc);

		// device binding?
		if (nTrack->Attribute("target_device") != 0) {
			RackElement* re = idToElement[nTrack->IntAttribute("target_device")];
			NoteDevice* ne = dynamic_cast<NoteDevice*>(re);
			if (!ne) {throw WorkspaceLoaderException("element is not a note device!");}
			st.setDevice(ne);
		}

		// load all events
		XMLElement* nEvents = nTrack->FirstChildElement("Events");
		if (!nEvents) {throw WorkspaceLoaderException("track has no events!");}

		// load all events
		for (
			 XMLElement* nEvent = nEvents->FirstChildElement("Event");
			 nEvent;
			 nEvent = nEvent->NextSiblingElement("Event")
			 ) {

			// get event params
			int time = nEvent->IntAttribute("time");
			int status = nEvent->IntAttribute("status");
			int d1 = nEvent->IntAttribute("d1");
			int d2 = nEvent->IntAttribute("d2");

			// add event
			MidiEvent evt(time, (uint8_t) status, (uint8_t) d1, (uint8_t) d2);
			st.addEvent(evt);

		}


		// add track
		ctx.getSequencer()->addTrack(st);

	}

}


