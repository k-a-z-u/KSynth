#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

class Context;
class RackElement;

class Controller{

public:

	/** ctor */
	Controller(Context& ctx);

	/** close the application */
	void exit();


	/** save the current workspace */
	void save();

	/** load a stored workspace */
	void load();


	/** add a new element to the rack */
	void addNewRackElement(const std::string str);

	/** remove an existing element from the rack */
	void removeRackElement(RackElement* el);

	/** reset the rack to an initial state */
	void resetRack();


	/** import a new midi file */
	void importMidi();

	/** remove all tracks from the sequencer */
	void clearTracks();

private:

	/** the main context */
	Context& ctx;

};

#endif // CONTROLLER_H
