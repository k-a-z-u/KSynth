/*
 * PinProperties.h
 *
 *  Created on: Jan 7, 2014
 *      Author: kazu
 */

#ifndef PINPROPERTIES_H_
#define PINPROPERTIES_H_

#include <string>

/**
 * describes the properties of one pin which
 * represents one input / output of one device.
 *
 * e.g.	channel 1 left input
 * 		channel 1 right input
 */
struct PinProperties {

	/** the pin's name */
	std::string name;

};


#endif /* PINPROPERTIES_H_ */
