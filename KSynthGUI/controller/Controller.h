#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

class Context;
class RackElement;
class SequencerTrack;


class Controller {

public:

	/** ctor */
	Controller(Context& ctx);

	/** close the application */
	void exit();


	/** save the current workspace */
	void save();

	/** load a stored workspace */
	void load();


	/** show the settings dialog */
	void showSettings();

	/** load settings from file */
	void loadSettings();

	/** save settings to file */
	void saveSettings();


	/** add a new element to the rack */
	void addNewRackElement(const std::string str);

	/** remove an existing element from the rack */
	void removeRackElement(RackElement* el);

	/** reset the rack to an initial state */
	void resetRack();



	/** add a new track to the sequencer */
	void addNewTrack();

	/** delete the currently selected track */
	void deleteSelectedTrack();


	/** import a new midi file */
	void importMidi();

	/** export the current song */
	void exportSong();


	/** remove all tracks from the sequencer */
	void clearTracks();


	/** show an error message */
	void messageError(const std::string& title, const std::string& text);

private:

	/** the main context */
	Context& ctx;

};

#endif // CONTROLLER_H
