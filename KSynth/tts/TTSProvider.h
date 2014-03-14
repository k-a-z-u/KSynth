#ifndef TTSPROVIDER_H
#define TTSPROVIDER_H

#include <string>
#include "../sampler/Sample.h"
#include <exception>

/** exception handling */
class TTSProviderException : public std::exception {
public:
	TTSProviderException(const std::string& msg) : msg(msg) {;}
	const char* what() const throw() {return msg.c_str();}
private:
	std::string msg;
};

/**
 * interface for all TSS providers.
 */
class TTSProvider {

public:

	/** dtor */
	virtual ~TTSProvider() {;}

	/** speak the given text into a new Sample */
	virtual Sample speak(const std::string& text) = 0;

};

#endif // TTSPROVIDER_H
