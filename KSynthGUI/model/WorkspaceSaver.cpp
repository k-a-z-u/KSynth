#include "WorkspaceSaver.h"

#include <KSynth/misc/SoundBase.h>
#include <KSynth/Generator.h>

#include <KLib/streams/FileOutputStream.h>
#include <KLib/streams/GzipOutputStream.h>

#include "../lib/tinyxml2/tinyxml2.h"

#include "../rack/RackElement.h"
#include "../model/Context.h"
#include "../rack/Rack.h"


WorkspaceSaver::WorkspaceSaver(Context& ctx) : ctx(ctx) {
	;
}

void WorkspaceSaver::save(const K::File& file) {

	// create xml
	std::string xml = getXML();

	// open file
	std::string ext = file.getExtensionMulti();
	K::FileOutputStream fos(file);

	// export
	if (ext == "xml") {
		fos.write( (uint8_t*) xml.c_str(), (unsigned int) xml.length());
		fos.close();

	} else if (ext == "xml.gz") {
#ifdef WITH_ZLIB
		K::GzipOutputStream gos(fos, K::GzipOutputStreamHeader::MODE_GZIP, 9);
		gos.write( (uint8_t*) xml.c_str(), (unsigned int) xml.length());
		gos.close();
#else
		throw WorkspaceSaverException("can't save compressed XML as zlib support is not compiled!");
#endif

	} else {
		throw WorkspaceSaverException("unsupported file-format: " + ext);

	}

}

std::string WorkspaceSaver::getXML() {

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* nRoot = doc.NewElement("KSynth");

	tinyxml2::XMLElement* nSettings = doc.NewElement("Settings");		// workspace settings (speed, etc)
	tinyxml2::XMLElement* nRack = doc.NewElement("Rack");
	tinyxml2::XMLElement* nBindings = doc.NewElement("Bindings");
	tinyxml2::XMLElement* nTracks = doc.NewElement("Tracks");

	doc.InsertEndChild(nRoot);
	nRoot->InsertEndChild(nSettings);
	nRoot->InsertEndChild(nRack);
	nRoot->InsertEndChild(nBindings);
	nRoot->InsertEndChild(nTracks);

	// export settings
	addSettings(&doc, nSettings);

	// export each rack element
	for (const RackElement* elem : ctx.getRack()->getElements()) {

		// assign and ID to each element
		if (ids.find(elem) == ids.end()) {ids[elem] = (unsigned int) ids.size();}

		// store
		add(elem, &doc, nRack);

	}

	// export all bindings
	for (const Binding& b : ctx.getGenerator()->getBinder().getBindings()) {
		add(b, &doc, nBindings);
	}

	// export all tracks
	for ( auto& st : ctx.getSequencer()->getTracks() ) {
		add(*st, &doc, nTracks);
	}

	// create XML
	tinyxml2::XMLPrinter pr;
	doc.Print( &pr );
	return std::string(pr.CStr());

}

void WorkspaceSaver::add(const RackElement* elem, tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* nRack) {

	tinyxml2::XMLElement* nElement = doc->NewElement("RackElement");
	nRack->InsertEndChild(nElement);

	// cast to SoundBase
	const SoundBase* sb = dynamic_cast<const SoundBase*>(elem);
	if (!sb) {return;}

	nElement->SetAttribute("type", sb->getProductString().c_str());
	nElement->SetAttribute("user_name", sb->getUserName().c_str());
	nElement->SetAttribute("id", ids[elem]);

	tinyxml2::XMLElement* nParams = doc->NewElement("Parameters");
	nElement->InsertEndChild(nParams);

	for (unsigned int i = 0; i < sb->getNumParameters(); ++i) {
		tinyxml2::XMLElement* nParam = doc->NewElement("Parameter");
		nParams->InsertEndChild(nParam);
		nParam->SetAttribute("num", i);
		nParam->SetAttribute("name", sb->getParameterName(i).c_str());
		nParam->SetAttribute("value", sb->getParameter(i));
	}


}

void WorkspaceSaver::addSettings(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement* nSettings) {

	tinyxml2::XMLElement* nSeq = doc->NewElement("Sequencer");
	nSeq->SetAttribute("bpm", ctx.getSequencer()->getBeatsPerMinute());

	nSettings->InsertEndChild(nSeq);

}

void WorkspaceSaver::add(const Binding &b, tinyxml2::XMLDocument *doc, tinyxml2::XMLElement* nBindings) {

	tinyxml2::XMLElement* nBinding = doc->NewElement("Binding");
	nBindings->InsertEndChild(nBinding);

	nBinding->SetAttribute("src_id", ids[ dynamic_cast<RackElement*>(b.src.dev) ]);
	nBinding->SetAttribute("src_port", b.src.idx);
	nBinding->SetAttribute("dst_id", ids[ dynamic_cast<RackElement*>(b.dst.dev) ]);
	nBinding->SetAttribute("dst_port", b.dst.idx);

}

void WorkspaceSaver::add(SequencerTrack& st, tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* nTracks) {

	// create entry for track
	tinyxml2::XMLElement* nTrack = doc->NewElement("Track");
	nTracks->InsertEndChild(nTrack);

	// set params
	nTrack->SetAttribute("name", st.getName().c_str());
	if (st.getDevice()) {nTrack->SetAttribute("target_device", ids[ dynamic_cast<RackElement*>(st.getDevice()) ]);}

	// track's description
	tinyxml2::XMLElement* nDesc = doc->NewElement("Desc");
	tinyxml2::XMLText* tDesc = doc->NewText( st.getDescription().c_str() );
	nTrack->InsertEndChild(nDesc);
	nDesc->InsertEndChild(tDesc);

	// holder for midi events
	tinyxml2::XMLElement* nEvts = doc->NewElement("Events");
	nTrack->InsertEndChild(nEvts);

	// now export all midi events
	for (const MidiEvent* evt : *st.getEvents()) {
		tinyxml2::XMLElement* nEvt = doc->NewElement("E");
		nEvts->InsertEndChild(nEvt);
		nEvt->SetAttribute("t", evt->getDelay());
		nEvt->SetAttribute("s", evt->getStatus());
		nEvt->SetAttribute("d1", evt->getData1());
		nEvt->SetAttribute("d2", evt->getData2());
	}

}
