#ifndef MIDIBINDER_H
#define MIDIBINDER_H

#include "../misc/DataTypes.h"
#include "MidiBinding.h"
#include "rt/RtMidiEventListener.h"

#include <vector>
#include <algorithm>
#include <exception>
#include <string>

/** exception handling */
class MidiBinderException : public std::exception {
public:
	MidiBinderException(const std::string& msg) : msg(msg) {;}
	const char* what() const throw() {return msg.c_str();}
private:
	std::string msg;
};


/**
 * the MidiBinder handles bindings (routings) between
 * midi events from external midi devices (keyboards, etc)
 * to SoundDevices (synthesizers, samplers, ..).
 *
 * using those bindings its posible to use a midi controlChange
 * (like spinning a knob, moving a slider) to modify a SoundDevice's
 * parameters like the main volume.
 *
 * the binder is attached to the RtMidiWrapper, receives events from all
 * attached midi devices and trigges all registered bindings.
 *
 */
class MidiBinder : public RtMidiEventListener {

public:

	/** ctor */
	MidiBinder() {;}

	/** add a new binding */
	void addBinding(const MidiBinding& mb) {

		// sanity check: bindings must not appear more than once
		if (hasBinding(mb.dst.dev, mb.dst.param)) {throw MidiBinderException("this parameter is already bound!");}

		// add binding
		bindings.push_back(mb);

	}

	/**
	 * remove an existing binding for the given target combination
	 * @param dev the event target
	 * @param p the parameter to change within the target
	 */
	void removeBinding(const SoundBase* dev, const Param p) {
		auto match = [dev, p] (const MidiBinding& b) {return b.dst.dev == dev && b.dst.param == p;};
		bindings.erase( std::remove_if(bindings.begin(), bindings.end(), match), bindings.end());
	}


	/**
	 * check wheter SoundBase's parameter p is already bound
	 * @param dev the device to check for an existing binding
	 * @param p the parameter the check for a binding
	 * @return
	 */
	bool hasBinding(const SoundBase* dev, const Param p) const {
		auto match = [dev, p] (const MidiBinding& mb) {return mb.dst.dev == dev && mb.dst.param == p;};
		return std::find_if(bindings.begin(), bindings.end(), match) != bindings.end();
	}

protected:

	void onMidiEvent(RtMidiInDevice& src, MidiEvent evt) override {

		// check all bindings for a match
		for (MidiBinding& b : bindings) {

			// match? -> adjust params
			if (b.matches(src, evt)) {
				b.dst.dev->setParameter(b.dst.param, evt.getData2AsFloat());
			}

		}

	}

private:

	/** all registered midi bindings */
	std::vector<MidiBinding> bindings;

};

#endif // MIDIBINDER_H
