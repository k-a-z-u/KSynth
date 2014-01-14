#ifndef WORKSPACESAVER_H
#define WORKSPACESAVER_H

#include <KLib/fs/File.h>
#include "../lib/tinyxml2/tinyxml2.h"

class RackElement;
class Context;
class Binding;
class SoundBase;
class SequencerTrack;

#include <unordered_map>

class WorkspaceSaver {

public:

	/** ctor */
	WorkspaceSaver(Context& ctx);

	/** export the current Rack to xml */
	void save(const K::File& file);


private:

	/** export the given RackElement */
	void add(const RackElement* elem, tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* node);

	/** export the given binding */
	void add(const Binding& b, tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* node);

	/** export the given SequencerTrack */
	void add(const SequencerTrack& st, tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* node);

	/** export workspace settings */
	void addSettings(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement* nSettings);

	Context& ctx;

	/** assign an id to each rack element */
	std::unordered_map<const RackElement*, unsigned int> ids;

};

#endif // WORKSPACE_H
