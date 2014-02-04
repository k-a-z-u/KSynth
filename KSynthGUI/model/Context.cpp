#include "Context.h"
#include "../rack/Rack.h"
#include "../rack/RackFactory.h"
#include "../rack/MasterTarget1.h"
#include "../controller/Controller.h"
#include "../controller/tasks/Tasks.h"
#include "../MainWin.h"
#include "../MidiBindingDialog.h"

#include "model/SoundSinks.h"
#include "controller/SongExport.h"

#include <KSynth/output/MasterTarget.h>

#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>
#include <KSynth/pattern/PatternSequencer.h>
#include <KSynth/mixer/SimpleMixer.h>
#include <KSynth/midi/MidiBinder.h>

#include <QApplication>
#include <QMessageBox>

Context::Context(QApplication& app) :
	app(app), fmt(2,48000,16), dst(nullptr), tasks(nullptr), midiBinder(nullptr) {

	ctrl = new Controller(*this);
	seq = new Sequencer();
	gen = new Generator(fmt);

	gen->setSequencer(seq);
	gen->setSource(nullptr);

	// the main window
	mainWindow = new MainWin(*this);
	mainWindow->show();

	// the factory to create RackElements by a type-name
	rackFactory = new RackFactory(*this);

	// set-up the midi backend
	midi = new RtMidiWrapper();
	try {

		// load all midi devices
		midi->init();

		// create binder for MidiEvent->ParamChange
		// and add the binder as event listener
		midiBinder = new MidiBinder();
		midi->addListener(midiBinder);

	} catch (RtMidiException& e) {
		QMessageBox::information(mainWindow, "midi system",
								 ("initialization of the midi subsystem failed.\n"
								  "you won't be able to use external midi devices!\n\n"
								  "error message was:\n\n" + std::string(e.what())
								  ).c_str()
								 );
	}

	// dialog for binding midi events
	midiBindingDialog = new MidiBindingDialog(*this, mainWindow);

	// song exporter
	songExp = new SongExport(*this);

	// settings object
	settings = new SystemSettings(*this);

}

Context::~Context() {

	// stop playback
	gen->stop();

	// delete background task system
	delete tasks;				tasks = nullptr;	std::cout << 1 << std::endl;

	// delete midi related stuff
	delete midiBindingDialog;	midiBindingDialog = nullptr; std::cout << 2 << std::endl;
	delete midiBinder;			midiBinder = nullptr; std::cout << 3 << std::endl;
	delete midi;				midi = nullptr; std::cout << 4 << std::endl;

	dst = nullptr;				// the Rack will delete this device!
	delete mainWindow;			mainWindow = nullptr; std::cout << 5 << std::endl;

	delete ctrl;				ctrl = nullptr; std::cout << 6 << std::endl;
	delete gen;					gen = nullptr; std::cout << 7 << std::endl;
	delete seq;					seq = nullptr; std::cout << 8 << std::endl;
	delete rackFactory;			rackFactory = nullptr; std::cout << 9 << std::endl;
	delete rack;				rack = nullptr; std::cout << 10 << std::endl;

	// song exporter
	delete songExp;				songExp = nullptr; std::cout << 11 << std::endl;

	// system settings
	delete settings;			settings = nullptr; std::cout << 12 << std::endl;

}

Rack* Context::getRack() const {
	return rack;
}

void Context::setMasterTarget(MasterTarget1 *sb) {
	this->dst = sb;
	this->gen->setSource(sb);
	if (sb) { sb->setUserName("Master");}
}

QApplication* Context::getApplication() const {
	return &app;
}

Generator* Context::getGenerator() const {
	return gen;
}

Sequencer* Context::getSequencer() const {
	return seq;
}

Controller* Context::getController() const {
	return ctrl;
}

RackFactory* Context::getRackFactory() const {
	return rackFactory;
}

Tasks* Context::getTasks() const {
	return tasks;
}

MasterTarget1* Context::getMasterTarget() const {
	return dst;
}

RtMidiWrapper* Context::getMidi() const {
	return midi;
}

MidiBinder* Context::getMidiBinder() const {
	return midiBinder;
}

MidiBindingDialog* Context::getMidiBindingDialog() const {
	return midiBindingDialog;
}

MainWin* Context::getMainWindow() const {
	return mainWindow;
}

SystemSettings* Context::getSettings() const {
	return settings;
}

SongExport* Context::getSongExporter() const {
	return songExp;
}
