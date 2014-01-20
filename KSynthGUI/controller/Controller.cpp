#include "Controller.h"

#include <QApplication>
#include <QMessageBox>

#include "../model/Context.h"
#include "../rack/RackElement.h"
#include "../rack/RackFactory.h"
#include "../rack/Rack.h"
#include "RackWidget.h"
#include "../model/WorkspaceSaver.h"
#include "../model/WorkspaceLoader.h"
#include "CtrlHelper.h"

#include <KSynth/Generator.h>
#include <KSynth/Sequencer.h>
#include <KSynth/midi/MidiParser.h>

#include <KLib/fs/File.h>

Controller::Controller(Context& ctx) : ctx(ctx) {
	;
}

void Controller::exit() {
	ctx.getGenerator()->stop();
	ctx.getRack()->setRefreshing(false);
	QApplication::quit();
}

void Controller::save() {

	// supported formats
	std::vector<FileDialogFilter> formats;
	formats.push_back( FileDialogFilter("KSynth Workspace", "xml") );
#ifdef WITH_ZLIB
	formats.push_back( FileDialogFilter("KSynth Workspace (compressed)", "xml.gz") );
#endif

	std::string file = CtrlHelper::saveFile("Save Workspace", formats);
	if (!file.empty()) {
		WorkspaceSaver ws(ctx);
		ws.save( K::File(file) );
	}
}

#include "../controller/tasks/Tasks.h"
void Controller::load() {

	// supported formats
	std::vector<FileDialogFilter> formats;
	formats.push_back( FileDialogFilter("KSynth Workspace", "xml") );

	// zlib support?
	#ifdef WITH_ZLIB
	formats.push_back( FileDialogFilter("KSynth Workspace (compressed)", "xml.gz") );
	#endif

	// show file-open dialog
	std::string file = CtrlHelper::openFile("Load Workspace", formats);
	if (file.empty()) {return;}

	// execute
	class MyTask : public Task {

		Context& ctx;
		const std::string& file;

	public:

		MyTask(Context& ctx, const std::string& file) :
				Task("loading workspace", true), ctx(ctx), file(file) {;}

		void exec() {

			WorkspaceLoader wl(ctx);
			StatusCallback sc = [this] (const std::string& msg, float val) {setProgress(msg, val);};

			// load
			try {
				wl.load( K::File(file), sc );
			} catch (std::exception& e) {
				QMessageBox::critical(0, "error while loading file", e.what());
			}

		}

	};

	// run
	ctx.getTasks()->addTaskForeground( new MyTask(ctx, file) );

}



void Controller::addNewRackElement(const std::string str) {
	RackElement* re = ctx.getRackFactory()->getByString(str, ctx);
	ctx.getRack()->add(re);
}

void Controller::removeRackElement(RackElement* re) {
	ctx.getRack()->remove(re);
}

void Controller::resetRack() {
	ctx.getGenerator()->stop();
	ctx.getRack()->reset();
}

void Controller::clearTracks() {
	ctx.getSequencer()->clearTracks();
}


void Controller::addNewTrack() {
	ctx.getSequencer()->addTrack(SequencerTrack());
}

void Controller::importMidi() {
	try {
		std::string file = CtrlHelper::openFile("Import MIDI-File", FileDialogFilter("MIDI-File", "mid") );
		if (!file.empty()) {
			MidiFile midi;
			MidiParser( K::File(file), midi );
			ctx.getSequencer()->import(midi, 0);
		}
	} catch (...) {;}
}


