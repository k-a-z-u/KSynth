#include "Rack.h"

#include <thread>

#include <KSynth/notes/NoteDevice.h>
#include <KSynth/misc/SoundBase.h>
#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>

#include "../RackWidget.h"
#include "../model/Context.h"
#include "../rack/MasterTarget1.h"

Rack::Rack(Context& ctx, RackWidget& rw) : ctx(ctx), rw(rw), running(true), refreshing(true) {

	// create refresh thread
	thread = new std::thread(&Rack::run, this);

}

Rack::~Rack() {

	setRefreshing(false);

	//	// remove all elements
	//	mutex.lock();
	//	for (RackElement* el : elems) {delete el;}
	//	elems.clear();
	//	mutex.unlock();

	// stop refresh thread
	running = false;
	thread->join();
	delete thread;

}

void Rack::setRefreshing(bool enabled) {
	mutex.lock();
	refreshing = enabled;
	mutex.unlock();
}


bool Rack::hasElementNamed(const std::string& name) const {
	for (RackElement* re : elems) {
		std::string name2 = re->getUserName();
		if (name2 == name) {return true;}
	}
	return false;
}

void Rack::add(RackElement* re) {

	// empty user name?
	if (re->getUserName().empty()) {re->setUserName(re->getProductString());}

	// ensure the element provides an unique name
	if (hasElementNamed(re->getUserName())) {

		// create a unique name
		std::string name = "";
		for (unsigned int i = 1; i < 64; ++i) {
			name = re->getProductString() + " " + std::to_string(i);
			if (!hasElementNamed(name)) {break;}
		}
		re->setUserName(name);

	}

	// add to gui
	rw.add(re);

	// add to list
	elems.push_back(re);

	// user name changed -> rack changed
	connect(re, SIGNAL(userNameChanged()), this, SIGNAL(onRackChanged()));

	// attach
	re->onRackAttach();

	emit entryAdded(elems.size() - 1);
	emit onRackChanged();

}

void Rack::remove(RackElement* re) {

	// detach
	re->onRackDetach();

	int idx = 0;
	mutex.lock(); {

		// remove from visible rack
		rw.remove(re);

		// remove from refreshing thread
		//auto match = [el] (RackElement* re) {return el == re;};
		//elems.erase( std::remove_if(elems.begin(), elems.end(), match), elems.end() );
		for (unsigned int i = 0; i < elems.size(); ++i) {
			if (elems[i] == re) {idx = i; elems.erase(elems.begin() + i); break;}
		}

	} mutex.unlock();

	// if i am a note-device, unbind me from all tracks
	NoteDevice* nd = dynamic_cast<NoteDevice*>(re);
	if (nd) {ctx.getSequencer()->unbind(nd);}

	// remove all my bindings
	SoundBase* sb = dynamic_cast<SoundBase*>(re);
	ctx.getGenerator()->getBinder().deleteAllFor(sb);

	// remove from ram
	re->deleteLater();

	emit entryRemoved(idx);
	emit onRackChanged();

}


void Rack::reset() {

	unsigned int cnt = elems.size();
	while (cnt--) {
		RackElement* re = elems[0];
		remove(elems[0]);
		re->deleteMe();
	}

}

void Rack::run() {

	while (running) {

		std::chrono::milliseconds dur(70);
		std::this_thread::sleep_for( dur );

		// refresh each registered element
		if (refreshing) {
			mutex.lock();
			for (RackElement* e : elems) { QMetaObject::invokeMethod(e, "refresh", Qt::QueuedConnection); }
			mutex.unlock();
		}

	}

}

const std::vector<RackElement*>& Rack::getElements() const {
	return elems;
}

std::vector<SoundBase*> Rack::getSoundBaseDevices() const {
	std::vector<SoundBase*> vec;
	for (RackElement* re : elems) {
		SoundBase* dev = dynamic_cast<SoundBase*>(re);
		if (dev) {vec.push_back(dev);}
	}
	return vec;
}

std::vector<NoteDevice*> Rack::getNoteDevices() const {
	std::vector<NoteDevice*> vec;
	for (RackElement* re : elems) {
		NoteDevice* dev = dynamic_cast<NoteDevice*>(re);
		if (dev) {vec.push_back(dev);}
	}
	return vec;
}
