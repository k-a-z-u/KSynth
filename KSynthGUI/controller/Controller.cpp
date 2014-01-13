#include "Controller.h"

#include "../model/Context.h"
#include <QApplication>
#include "../rack/RackElement.h"
#include "../rack/RackFactory.h"
#include "../rack/Rack.h"
#include "RackWidget.h"
#include "../model/WorkspaceSaver.h"
#include "../model/WorkspaceLoader.h"
#include "CtrlHelper.h"

#include <KSynth/Sequencer.h>
#include <KSynth/midi/MidiParser.h>

#include <klib/fs/File.h>

Controller::Controller(Context& ctx) : ctx(ctx) {
	;
}

void Controller::exit() {
	ctx.getRack()->setRefreshing(false);
	QApplication::quit();
}

void Controller::save() {
	std::string file = CtrlHelper::saveFile("Save Workspace", "KSynth Workspace", "xml");
	if (!file.empty()) {
		WorkspaceSaver ws(ctx);
		ws.save( K::File(file) );
	}
}

void Controller::load() {
	std::string file = CtrlHelper::openFile("Load Workspace", "KSynth Workspace", "*.xml");
	if (!file.empty()) {
		WorkspaceLoader wl(ctx);
		wl.load( K::File(file) );
	}
}

void Controller::addNewRackElement(const std::string str) {
	RackElement* re = ctx.getRackFactory()->getByString(str, ctx);
	ctx.getRack()->add(re);
}

void Controller::removeRackElement(RackElement* re) {
	ctx.getRack()->remove(re);
}

void Controller::resetRack() {
	ctx.getRack()->reset();
}

void Controller::clearTracks() {
	ctx.getSequencer()->clearTracks();
}

void Controller::importMidi() {
	try {
		std::string file = CtrlHelper::openFile("Import MIDI-File", "MIDI-File", "*.mid");
		if (!file.empty()) {
			MidiFile midi;
			MidiParser( K::File(file), midi );
			ctx.getSequencer()->import(midi, 0);
		}
	} catch (...) {;}
}
