#ifndef CONTEXT_H
#define CONTEXT_H


#include <KSynth/misc/AudioFormat.h>
#include "SystemSettings.h"

class RackFactory;
class MasterTarget1;
class SoundSink;
class AudioFormat;
class Sequencer;
class Generator;
class Controller;
class QApplication;
class Rack;
class SoundBase;
class Tasks;
class RtMidiWrapper;
class MidiBinder;
class MidiBindingDialog;
class MainWin;

/**
 * this clas describes the application's context and provides access
 * to all elements needed in almost any place of the application.
 */
class Context {

public:

	/** ctor */
	Context(QApplication& app);

	/** dtor */
	~Context();


	/** get the factory to create RackElements */
	RackFactory* getRackFactory() const;

	/** get the main controller */
	Controller* getController() const;

	/** get the main application */
	QApplication* getApplication() const;

	/** get the RackWidget */
	Rack* getRack() const;

	/** get the main sequencer */
	Sequencer* getSequencer() const;

	/** get the generator */
	Generator* getGenerator() const;

	/** set the master target */
	void setMasterTarget(MasterTarget1* sb);

	/** get facility for foreground/background tasks */
	Tasks* getTasks() const;

	/** get the master target to send audio to */
	MasterTarget1* getMasterTarget() const;

	/** get the midi subsystem to communicate with midi devices */
	RtMidiWrapper* getMidi() const;

	/** the binder to glue midi events to device parameters */
	MidiBinder* getMidiBinder() const;

	/** get the convenience dialog automatically creating midi bindings */
	MidiBindingDialog* getMidiBindingDialog() const;

	/** get the main application window */
	MainWin* getMainWindow() const;

	/** get the system settings manager */
	SystemSettings* getSettings() const;

private:

	/** all system settings reside here */
	SystemSettings* settings;


	/** the main sequencer */
	Sequencer* seq;

	/** the main generator */
	Generator* gen;

	/** the main Rack */
	friend int main(int argc, char** argv);
	Rack* rack;

	/** the main application */
	QApplication& app;

	/** audio format settings */
	AudioFormat fmt;

	/** the main controller */
	Controller* ctrl;

	/** factory to create RackElements */
	RackFactory* rackFactory;

	/** the main target */
	MasterTarget1* dst;

	/** execute foreground/background tasks */
	Tasks* tasks;

	/** access external midi devices */
	RtMidiWrapper* midi;

	/** attach/detach midi bindings here */
	MidiBinder* midiBinder;

	/** dialog to bind midi events */
	MidiBindingDialog* midiBindingDialog;

	/** the main application window */
	MainWin* mainWindow;



};

#endif // CONTEXT_H
