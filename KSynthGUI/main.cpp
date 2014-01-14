

#include <QApplication>
#include <QMessageBox>

#include <KSynth/midi/rt/RtMidiWrapper.h>
#include <KSynth//midi/rt/RtMidiInDevice.h>
#include <KSynth/midi/MidiParser.h>
#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>

#include "SynthWin.h"
#include "SequencerWidget.h"
#include "RackWidget.h"
#include "MainWin.h"
#include "ControlPanelWidget.h"
#include "controller/Controller.h"

#include "rack/Rack.h"
#include "rack/RackFactory.h"
#include "rack/Synth1.h"
#include "rack/Sequencer1.h"
#include "rack/Reverb1.h"
#include "rack/Mixer1.h"
#include "rack/DrumComputer1.h"
#include "rack/MasterTarget1.h"




void showError(std::string err) {
	err += "\n\nthe program will now terminate!";
	int ret = QMessageBox::critical(nullptr, QString("an exception occurred..."), QString(err.c_str()), QMessageBox::Ok, QMessageBox::Ok);
	(void) ret;
	exit(-1);
}

/**
 * @brief Application wrapper to catch exceptions
 */
class MyApplication : public QApplication {


public:

	/** ctor */
	MyApplication(int& argc, char** argv) : QApplication(argc, argv) {;}

	/** dtor */
	virtual ~MyApplication() {;}

	/** catch exceptions */
	virtual bool notify(QObject* receiver, QEvent* event) {
		try {
			return QApplication::notify(receiver, event);

		} catch(std::exception& e) {
			showError(e.what());

		} catch (...) {
			showError("an unknown error occured.");

		}

		return false;
	}

};

#include <SampleChooserDialog.h>

int main(int argc, char *argv[]) {

	MyApplication app(argc, argv);


	Context ctx(app);

	app.setStyleSheet(
				""
				);

	// the main window
	MainWin mw(ctx);
	mw.show();

	// create rack panel
	RackWidget rw(ctx);

	// create Rack-Model
	ctx.rack = new Rack(ctx, rw);

	// create sequencer panel
	SequencerWidget sw(ctx);

	// create control panel
	ControlPanelWidget cpw(ctx);

	// add master target
	MasterTarget1* dst = new MasterTarget1(ctx);
	ctx.setMasterTarget(dst);
	ctx.rack->add(dst);


	// add dock widgets
	mw.setUpdatesEnabled(false); {
		mw.addDockWidget(Qt::LeftDockWidgetArea, &rw);
		mw.addDockWidget(Qt::RightDockWidgetArea, &sw);
		mw.addDockWidget(Qt::BottomDockWidgetArea, &cpw);
	} mw.setUpdatesEnabled(true);


	mw.setUpdatesEnabled(false);

	/*

	DrumComputer1* dc1 = new DrumComputer1();

	Mixer1* m1 = (Mixer1*) ctx.getRackFactory()->getByString("SimpleMixer", ctx);// Mixer1();
	Mixer1* m2 = (Mixer1*) ctx.getRackFactory()->getByString("SimpleMixer", ctx);//new Mixer1();

	Synth1* s1 = new Synth1();
	Synth1* s2 = new Synth1();
	Synth1* s3 = new Synth1();
	Synth1* s4 = new Synth1();
	Synth1* s5 = new Synth1();
	Synth1* s6 = new Synth1();

	Reverb1* r1 = new Reverb1();

	Sequencer1* seq1 = new Sequencer1();
	seq1->setTarget(s1);
	Sequencer1* seq2 = new Sequencer1();
	seq2->setTarget(s2);



	rw.add(r1);

	rw.add(m1);
	rw.add(m2);

	rw.add(dc1);

	rw.add(s1);
	rw.add(seq1);

	rw.add(s2);
	rw.add(seq2);

	rw.add(s3);
	rw.add(s4);
	rw.add(s5);
	rw.add(s6);

	ctx.seq->bind(seq1);
	ctx.seq->bind(seq2);
	r1->bind(0, m1);

	m2->bind(0, s1);
	m2->bind(2, s2);
	m2->bind(4, s3);
	m2->bind(6, s4);
	m2->bind(8, s5);
	m2->bind(10, s6);

	m1->bind(0, m2);
	m1->bind(2, dc1);

	ctx.dst->bind(0, r1);

	MidiFile mid;
	int x = 0;

	if (x == 0) {
		MidiParser(K::File("/data/midi/duke1.mid"), mid);
		ctx.seq->import(mid, 0);
		ctx.seq->setBeatsPerMinute(85);
		//ctx.seq->bind(0, s1);
		ctx.seq->bind(1, s2);
		ctx.seq->bind(2, s3);
		ctx.seq->bind(3, s4);
		ctx.seq->bind(4, s5);
	} else if (x == 1) {
		MidiParser(K::File("/data/midi/superMario/smwintro.mid"), mid);
		ctx.seq->import(mid, 0);
		ctx.seq->setBeatsPerMinute(85);
		ctx.seq->bind(1, s1);
		ctx.seq->bind(2, s2);
		ctx.seq->bind(3, s3);
		ctx.seq->bind(4, s4);
		ctx.seq->bind(5, s5);
		ctx.seq->bind(6, s6);
	} else if (x == 2) {
		MidiParser(K::File("/data/midi/pattern-318.mid"), mid);
		ctx.seq->import(mid, 0);
		ctx.seq->setBeatsPerMinute(90);
		ctx.seq->bind(0, s1);
		ctx.seq->bind(1, s2);
		ctx.seq->bind(2, s3);
		ctx.seq->bind(3, s4);
		ctx.seq->bind(4, s5);
		ctx.seq->bind(5, s6);
	} else {
		ctx.seq->bind(0, s1);
		ctx.seq->setBeatsPerMinute(90);
	}
	*/

//	MidiFile midi;
//	MidiParser(K::File("/data/midi/fzero_intro.mid"), midi);
//	ctx.seq->import(midi,0);

	//ctx.getController()->addNewRackElement("SimpleSynth");
	//SoundBase* sb = ctx.getRack()->getElements().at(i)->
	Synth1* synth = new Synth1(ctx); synth->setUserName("synth1");
	Mixer1* mixer = new Mixer1(ctx); mixer->setUserName("mixer");
	DrumComputer1* drums = new DrumComputer1(ctx); drums->setUserName("drums");
	ctx.getRack()->add(synth);
	ctx.getRack()->add(mixer);
	ctx.getRack()->add(drums);
	ctx.gen->getBinder().addBinding(0, synth, 0, mixer);
	ctx.gen->getBinder().addBinding(1, synth, 1, mixer);
	ctx.gen->getBinder().addBinding(0, mixer, 0, dst);
	ctx.gen->getBinder().addBinding(1, mixer, 1, dst);

//	ctx.seq->bind(2, synth);
//	ctx.seq->bind(3, synth);
//	ctx.seq->bind(4, synth);
//	ctx.seq->bind(5, synth);


	ctx.seq->setBeatsPerMinute(90);
	mw.setUpdatesEnabled(true);

	return app.exec();

}
