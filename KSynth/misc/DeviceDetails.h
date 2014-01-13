/*
 * DeviceDetails.h
 *
 *  Created on: Jan 8, 2014
 *      Author: kazu
 */

#ifndef DEVICEDETAILS_H_
#define DEVICEDETAILS_H_

#include <string>

class DeviceDetails {

public:

	/** dtor */
	virtual ~DeviceDetails() {;}

	/** get a descriptive name for this element */
	virtual std::string getProductString() const = 0;

	/** set an unique, user-defined name for this element */
	virtual void setUserName(const std::string& name) {
		this->name = name;
	}

	/** get the user-defined name for this element */
	virtual const std::string& getUserName() const {
		return this->name;
	}

private:

	/** (unique?) user-defined device name */
	std::string name;

};



#endif /* DEVICEDETAILS_H_ */
