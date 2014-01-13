#ifndef RACKFACTORY_H
#define RACKFACTORY_H

class RackElement;
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include "../model/Context.h"
#include <exception>

/** exception handling */
class RackFactoryException : public std::exception {
public:
	RackFactoryException(const std::string& msg) : msg(msg) {;}
	const char* what() const throw() override {return msg.c_str();}
private:
	std::string msg;
};


/**
 * create RackElements by type
 */
class RackFactory {

public:

	/** dtor */
	~RackFactory();

	/** get list of all devices */
	std::vector<std::string> getDevices() const;

	/** get RackElement by String */
	RackElement* getByString(const std::string& str, Context& ctx) const;

private:

	friend class Context;

	/** hidden ctor */
	RackFactory(Context& ctx);

private:

	/** the context to use */
	Context& ctx;

	/** all available RackElements */
	std::unordered_map<std::string, std::unique_ptr<RackElement>> elems;

};

#endif // RACKFACTORY_H
