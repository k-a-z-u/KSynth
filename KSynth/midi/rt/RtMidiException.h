/*
 * RtMidiException.h
 *
 *  Created on: Dec 27, 2013
 *      Author: kazu
 */

#ifndef RTMIDIEXCEPTION_H_
#define RTMIDIEXCEPTION_H_

#include <string>
#include <exception>
#include "rtmidi/RtError.h"

class RtMidiException : public std::exception {

public:

	RtMidiException(const std::string& msg) : msg(msg) {;}

	RtMidiException(const std::string& msg, RtError& error) : msg(msg + "\n" + error.getMessage()) {;}

	virtual const char* what() const throw() {
		return msg.c_str();
	}

private:

	std::string msg;

};


#endif /* RTMIDIEXCEPTION_H_ */
