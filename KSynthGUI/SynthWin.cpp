#include "SynthWin.h"
#include "ui_SynthWin.h"

#include "rack/Synth1.h"
#include "rack/Sequencer1.h"
#include "rack/Reverb1.h"
#include "rack/Mixer1.h"
#include "rack/DrumComputer1.h"

#include <KSynth/fx/Reverb.h>
#include <KSynth/oscillator/OscillatorFunctions.h>
#include <KSynth/midi/MidiParser.h>

#include <KSynth/midi/rt/RtMidiWrapper.h>
#include <KSynth/midi/rt/devices/MidiDeviceLaunchkeyMini.h>


void midiTest() {

	RtMidiWrapper wrap;

	class X : public RtMidiEventListener {
	public: void onMidiEvent(RtMidiInDevice& src, MidiEvent evt) {
			Q_UNUSED(src);
			Q_UNUSED(evt);
			std::cout << "XX" << std::endl;
		}
	};

	X x;
	wrap.getInputs().at(0)->addListener(&x);

	wrap.getOutputs().at(1)->open();
	MidiDeviceLaunckeyMini lk(*wrap.getOutputs().at(1));
	lk.reset();
	//lk.setLEDTest(MidiDeviceLaunckeyMiniBrightness::HIGH);
	//while(true) {

	//	for (unsigned int i = 10; i < 127; ++i) {
	//		MidiEvent evt(0, 144, i, 0b11);
	//		wrap.getOutputs().at(1)->sendEvent(evt);
	//	}
	lk.setLED(MidiDeviceLaunckeyMiniLED::MIDI_CHANNEL, (int) MidiDeviceLaunckeyMiniColor::GREEN_LOW);

	//for (unsigned int val = 0; val <= 15; ++val) {
	for (unsigned int y = 0; y < 2; ++y) {
		for (unsigned int x = 0; x < 9; ++x) {
			lk.setLED(x,y, 3|(1<<2)|(1<<3));//MidiDeviceLaunckeyMiniColor::GREEN_LOW);
		}
	}
	//}
	//}

	//sleep(1000);


}



SynthWin::SynthWin(Context& ctx, QWidget *parent) :
	QMainWindow(parent), ui(new Ui::SynthWin), ctx(ctx) {
	ui->setupUi(this);

	this->setMinimumSize(660, 600);
	//this->setMaximumSize(660, 2000);

	//midiTest();






	//	// run
	//	ctx.gen->start();

}

SynthWin::~SynthWin() {
	delete ui;
}
