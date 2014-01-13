/*
 * SoundBase.h
 *
 *  Created on: Dec 6, 2013
 *      Author: kazu
 */

#ifndef SOUNDBASE_H_
#define SOUNDBASE_H_

#define SNDBASE_BLK_SIZE	256

#include "DataTypes.h"
#include "Events.h"
#include "PinProperties.h"
#include "DeviceDetails.h"

#include <string>
#include <cstring>
#include <vector>


//class SoundBase;
//
///**
// * one endpoint of a binding identified by a device and a port-index.
// * a binding consists of two endpoints: source and destination.
// */
//struct BindingEndpoint {
//
//	/** the port's index */
//	unsigned int idx;
//
//	/** the device */
//	SoundBase* dev;
//
//	/** equals? */
//	bool operator == (BindingEndpoint& be) const {
//		return (idx == be.idx) && (dev == be.dev);
//	}
//
//	/** ctor */
//	BindingEndpoint(unsigned int idx, SoundBase* dev) : idx(idx), dev(dev) {;}
//
//};
//
///**
// * represents one binding from outputPort@SourceDevice -> inputPort@DestinationDevice
// */
//struct Binding {
//
//	BindingEndpoint src;
//	BindingEndpoint dst;
//
//	/** equals? */
//	bool operator == (Binding& b) const {
//		return (src == b.src) && (dst == b.dst);
//	}
//
//	/** ctor */
//	Binding(unsigned int srcIdx, SoundBase* src, unsigned int dstIdx, SoundBase* dst) :
//		src(srcIdx, src), dst(dstIdx, dst) {;}
//
//};
//
//class BindingException : public std::exception {
//public:
//	BindingException(const std::string& msg) : msg(msg) {;}
//	const char* what() const throw() override {return msg.c_str();}
//private:
//	std::string msg;
//};


/**
 * base class for all sound generators and sound filters.
 *
 * provides some convenience methods, binding functions and
 * interface methods to implement.
 */
class SoundBase : virtual public DeviceDetails {

public:

	/** dtor */
	virtual ~SoundBase() {

		// de-allocate output buffer if any has been allocated
		// (the output buffer is lazy-loaded only if the plugin is part of a chain)
		if (outputs != nullptr) {
			for (unsigned int i = 0; i < numOutputs; ++i) {free(outputs[i]);}
			free(outputs);
			outputs = nullptr;
		}

		if (inputs != nullptr) {
			free(inputs);
			inputs = nullptr;
		}

	}

	/** take the given input (if any!) and provide the corresponding output */
	virtual void process(Amplitude** inputs, Amplitude** outputs) = 0;



	/** the number of input channels for this device */
	virtual unsigned int getNumInputs() {return numInputs;}

	/** the number of output channels for this device */
	virtual unsigned int getNumOutputs() {return numOutputs;}


	/** get properties for the input pin identified by idx */
	virtual bool getInputProperties(unsigned int idx, PinProperties* properties) {
		(void) idx;
		(void) properties;
		return false;
	}

	/** get properties for the output pin identified by idx */
	virtual bool getOutputProperties(unsigned int idx, PinProperties* properties) {
		(void) idx;
		(void) properties;
		return false;
	}



	/** how many samples to process on every process() call */
	SampleFrame getSamplesPerProcess() {return SNDBASE_BLK_SIZE;}

	/** the sample-rate used within the application */
	SampleRate getSampleRate() {return 48000;}


//
//	/**
//	 * add a new binding from srcIdx-th output of src to the dstIdx-th input of dst;
//	 * @param srcIdx the output channel of the source device
//	 * @param src the source device
//	 * @param dstIdx the input channel of the destination device
//	 * @param dst the destination device
//	 */
//	static void addBinding(unsigned int srcIdx, SoundBase* src, unsigned int dstIdx, SoundBase* dst) {
//
//		// check whether src is already bound
//		for (Binding& b : src->bindings) {
//			if (b.src.dev == src && b.src.idx == srcIdx) {
//				throw BindingException("source is already bound!");
//			}
//		}
//
//		// check whether dst is already bound
//		for (Binding& b : dst->bindings) {
//			if (b.dst.dev == dst && b.dst.idx == dstIdx) {
//				throw BindingException("destination is already bound!");
//			}
//		}
//
//		// create binding
//		Binding b(srcIdx, src, dstIdx, dst);
//
//		// add binding to both, source and destination
//		dst->bindings.push_back(b);
//		src->bindings.push_back(b);
//
//	}
//
//	/**
//	 * delete an existing binding from srcIdx-th output of src to the dstIdx-th input of dst;
//	 * @param srcIdx the output channel of the source device
//	 * @param src the source device
//	 * @param dstIdx the input channel of the destination device
//	 * @param dst the destination device
//	 */
//	static void deleteBinding(unsigned int srcIdx, SoundBase* src, unsigned int dstIdx, SoundBase* dst) {
//
//		Binding b(srcIdx, src, dstIdx, dst);
//
//		for (unsigned int i = 0; i < src->bindings.size(); ++i) {
//			if (src->bindings[i] == b) {src->bindings.erase(src->bindings.begin()+i); break;}
//		}
//
//		for (unsigned int i = 0; i < dst->bindings.size(); ++i) {
//			if (dst->bindings[i] == b) {dst->bindings.erase(dst->bindings.begin()+i); break;}
//		}
//
//	}
//
//
//
//	/** get a list of all bindings for this device */
//	std::vector<Binding> getBindings() {
//		return bindings;
//	}
//
//	/** get the device bound to my given output channel */
//	BindingEndpoint getBindingToOutputChannel(unsigned int idx) {
//		for (Binding& b : bindings) {
//			if (b.src.dev == this && b.src.idx == idx) {return b.dst;}
//		}
//		return BindingEndpoint(0, nullptr);
//	}
//
//	/** get the device bound to my given input channel */
//	BindingEndpoint getBindingToInputChannel(unsigned int idx) {
//		for (Binding& b : bindings) {
//			if (b.dst.dev == this && b.dst.idx == idx) {return b.src;}
//		}
//		return BindingEndpoint(0, nullptr);
//	}


	/** get the number of parameters this elements uses */
	virtual unsigned int getNumParameters() const = 0;

	/** set the value v (between [0,1]) of parameter p */
	virtual void setParameter(Param p, ParamValue v) {
		(void) p;
		(void) v;
	}

	/** get the value (between [0,1]) of a parameter p */
	virtual ParamValue getParameter(Param p) const {
		(void) p;
		return ParamValue(0.0f);
	}

	/** get the name (string) for the given parameter */
	virtual const std::string getParameterName(Param p) const {
		(void) p;
		return "NOT IMPLEMENTED";
	}

	/** set the given parameter's name */
	virtual void setParameterName(Param p, const std::string& name) {
		(void) p;
		(void) name;
	}

	/** VU meter */
	virtual Volume getVU() {return 0;}



protected:



	/** ctor: the number of input/output channels */
	SoundBase(unsigned int numInputs, unsigned int numOutputs) :
		outputs(nullptr), inputs(nullptr), numInputs(numInputs), numOutputs(numOutputs), hash(0) {

		// the output buffer will be lazy-loaded (malloced) only when
		// the plugin is part of a chain and data is fetched via get()

	}

	friend class Generator;

	/** create an output-buffer for this device (if not yet created) */
	void createOutputBuffer() {

		// already created?
		if (outputs != nullptr) {return;}

		// create buffer and sub-channels
		outputs = (Amplitude**) malloc(numOutputs * sizeof(Amplitude*));
		for (unsigned int i = 0; i < numOutputs; ++i) {
			outputs[i] = (Amplitude*) malloc(SNDBASE_BLK_SIZE * sizeof(Amplitude));
		}

		// sanity check
		if (!outputs) {throw std::exception();}

	}

	/** create a buffer for input-pointers */
	void createInputPointers() {

		// already created?
		if (inputs != nullptr) {return;}

		// array for all input-pointers to this device
		inputs = (Amplitude**) malloc(sizeof(Amplitude*) * numInputs);

	}


	/** fill all output channels with zeros */
	void resetOutputBuffer() {
		for (unsigned int i = 0; i < getNumOutputs(); ++i) {
			memset(outputs[i], 0, sizeof(Amplitude) * getSamplesPerProcess());
		}
	}

	/**
	 * reset all input pointers with nullptrs.
	 * this will ensure propper device-detaching when bindings have changed.
	 * else a once-set pointer will stay there forever!
	 */
	void resetInputPointers() {
		memset(inputs, 0, sizeof(Amplitude*) * numInputs);
	}






	/**
	 * called before process();
	 * inform the device it has to process the given events
	 * to affect its behavior for upcoming process() calls
	 */
	virtual void processEvents(const Events* evts) {
		(void) evts;
	}




private:

	/** output buffers for this device */
	Amplitude** outputs;

	/** input pointers for this device */
	Amplitude** inputs;


	/** the number of input channels */
	unsigned int numInputs;

	/** the number of output channels */
	unsigned int numOutputs;


	/** "hash" value to ensure the output is only rendered once */
	unsigned int hash;


	/** source bindings */
	//std::vector<Binding> bindings;



};

#endif /* SOUNDBASE_H_ */
