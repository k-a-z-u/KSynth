#include "Context.h"
#include "../rack/Rack.h"
#include "../rack/RackFactory.h"
#include "../rack/MasterTarget1.h"
#include "../controller/Controller.h"

#include <KSynth/output/SoundSinkAlsa.h>
#include <KSynth/output/SoundSinkWaveOut.h>
#include <KSynth/output/MasterTarget.h>

#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>
#include <KSynth/pattern/PatternSequencer.h>
#include <KSynth/mixer/SimpleMixer.h>

#include <QApplication>

Context::Context(QApplication& app) : app(app), fmt(2,48000,16) {

	ctrl = new Controller(*this);

#ifdef WITH_ALSA
	snd = new SoundSinkAlsa(fmt);
	//snd = new SoundSinkWave(K::File("/tmp/1.wav"), fmt);
	//snd = new SoundSinkMp3(K::File("/tmp/1.mp3"), fmt);
#elif WITH_COREAUDIO
	;
#elif WITH_WAVE_OUT
	snd = new SoundSinkWaveOut(fmt.sampleRate);
#endif

	seq = new Sequencer();
	gen = new Generator(fmt);

	gen->setSequencer(seq);
	gen->setSink(snd);
	gen->setSource(dst);

	rackFactory = new RackFactory(*this);

}

Context::~Context() {
	gen->stop();
	delete ctrl;		ctrl = nullptr;
	delete gen;			gen = nullptr;
	delete seq;			seq = nullptr;
	delete snd;			snd = nullptr;
	delete rackFactory;	rackFactory = nullptr;
	//delete dst;		// the Rack will delete this device!
	delete rack;		rack = nullptr;
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
