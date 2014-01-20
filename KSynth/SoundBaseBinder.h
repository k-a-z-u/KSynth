/*
 * SoundBaseBinder.h
 *
 *  Created on: Jan 9, 2014
 *      Author: kazu
 */

#ifndef SOUNDBASEBINDER_H_
#define SOUNDBASEBINDER_H_


#include "misc/SoundBase.h"

#include <vector>
#include <algorithm>

/**
 * one endpoint of a binding identified by a device and a port-index.
 * a binding consists of two endpoints: source and destination.
 */
struct BindingEndpoint {

	/** the port's index */
	unsigned int idx;

	/** the device */
	SoundBase* dev;

	/** equals? */
	bool operator == (BindingEndpoint& be) const {
		return (idx == be.idx) && (dev == be.dev);
	}

	/** ctor */
	BindingEndpoint(unsigned int idx, SoundBase* dev) : idx(idx), dev(dev) {;}

};


/**
 * represents one binding from outputPort@SourceDevice -> inputPort@DestinationDevice
 */
struct Binding {

	BindingEndpoint src;
	BindingEndpoint dst;

	/** equals? */
	bool operator == (Binding& b) const {
		return (src == b.src) && (dst == b.dst);
	}

	/** ctor */
	Binding(unsigned int srcIdx, SoundBase* src, unsigned int dstIdx, SoundBase* dst) :
		src(srcIdx, src), dst(dstIdx, dst) {;}

};


/** exception handling */
class BindingException : public std::exception {
public:
	BindingException(const std::string& msg) : msg(msg) {;}
	const char* what() const throw() override {return msg.c_str();}
private:
	std::string msg;
};


/**
 * this class handles pin-bindings (output -> input) between
 * all SoundBase devices.
 */
class SoundBaseBinder {

public:

	/**
	 * add a new binding from srcIdx-th output of src to the dstIdx-th input of dst;
	 * @param srcIdx the output channel of the source device
	 * @param src the source device
	 * @param dstIdx the input channel of the destination device
	 * @param dst the destination device
	 */
	void addBinding(unsigned int srcIdx, SoundBase* src, unsigned int dstIdx, SoundBase* dst) {

		// check whether src or dst is already bound
		for (Binding& b : bindings) {

			bool srcBound = b.src.dev == src && b.src.idx == srcIdx;
			bool dstBound = b.dst.dev == dst && b.dst.idx == dstIdx;

			if (srcBound || dstBound) {

				// get names for all endpoints for a clear error description
				PinProperties props;
				std::string srcName = b.src.dev->getProductString();
				std::string dstName = b.dst.dev->getProductString();
				b.src.dev->getOutputProperties(b.src.idx, &props); std::string srcPort = props.name;
				b.dst.dev->getInputProperties(b.dst.idx, &props); std::string dstPort = props.name;

				if (srcBound) {

					throw BindingException(
								"source port \n" +
								srcName + " : " + srcPort + "\n" +
								"is already bound to \n"+
								dstName + " : " + dstPort
								);
				}

				if (dstBound) {
					throw BindingException(
								"destination port \n" +
								dstName + " : " + dstPort + "\n" +
								"is already bound to \n" +
								srcName + " : " + srcPort + "\n"
								);
				}

			}

		}

		// create binding
		Binding b(srcIdx, src, dstIdx, dst);

		// add binding
		bindings.push_back(b);

	}

	/**
	 * delete all bindings involving the given device
	 * @param src the device to remove the bindings for
	 */
	void deleteAllFor(SoundBase* src) {
		auto match = [src] (Binding& b) {return b.dst.dev == src || b.src.dev == src;};
		bindings.erase( std::remove_if(bindings.begin(), bindings.end(), match), bindings.end() );
	}

	/**
	 * delete an existing binding from srcIdx-th output of src to the dstIdx-th input of dst;
	 * @param srcIdx the output channel of the source device
	 * @param src the source device
	 * @param dstIdx the input channel of the destination device
	 * @param dst the destination device
	 */
	void deleteBinding(unsigned int srcIdx, SoundBase* src, unsigned int dstIdx, SoundBase* dst) {

		Binding b(srcIdx, src, dstIdx, dst);

		for (unsigned int i = 0; i < bindings.size(); ++i) {
			if (bindings[i] == b) {
				bindings.erase(bindings.begin()+i); break;
			}
		}

	}


	/** get a list of all bindings */
	const std::vector<Binding>& getBindings() const {
		return bindings;
	}

	/** get all bindings to the inputs of dev */
	std::vector<Binding> getBindingsToInputsOf(SoundBase* dev) {
		std::vector<Binding> vec;
		for (Binding& b : bindings) {
			if (b.dst.dev == dev) {vec.push_back(b);}
		}
		return vec;
	}


	/** get the endpoint (if any) bound the the given devices idx-th output channel */
	BindingEndpoint getBindingToOutputChannel(SoundBase* dev, unsigned int idx) const {
		for (const Binding& b : bindings) {
			if (b.src.dev == dev && b.src.idx == idx) {return b.dst;}
		}
		return BindingEndpoint(0, nullptr);
	}

	/** get the endpoint (if any) bound the the given devices idx-th output channel */
	BindingEndpoint getBindingToOutputChannel(BindingEndpoint be) const {
		return getBindingToOutputChannel(be.dev, be.idx);
	}


	/** get the endpoint (if any) bound the the given devices idx-th input channel */
	BindingEndpoint getBindingToInputChannel(SoundBase* dev, unsigned int idx) const {
		for (const Binding& b : bindings) {
			if (b.dst.dev == dev && b.dst.idx == idx) {return b.src;}
		}
		return BindingEndpoint(0, nullptr);
	}

	/** get the endpoint (if any) bound the the given devices idx-th input channel */
	BindingEndpoint getBindingToInputChannel(BindingEndpoint be) const {
		return getBindingToInputChannel(be.dev, be.idx);
	}

private:

	/** source bindings */
	std::vector<Binding> bindings;

};


#endif /* SOUNDBASEBINDER_H_ */
