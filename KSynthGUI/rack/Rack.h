#ifndef RACK_H
#define RACK_H

#include <vector>
#include <QObject>
#include "RackElement.h"

namespace std {
	class thread;
}
class RackWidget;
class NoteDevice;
class SoundBase;
class Context;
#include <mutex>

class Rack : public QObject {
	Q_OBJECT

public:

	/** ctor */
	Rack(Context& ctx, RackWidget& rw);

	/** dtor */
	virtual ~Rack();


	/** add the given element to the rack */
	void add(RackElement* el);

	/** remove the given element from the rack */
	void remove(RackElement* el);

	/** reset the rack to an initial state */
	void reset();


	/** enable / disable refreshing */
	void setRefreshing(bool enabled);

	/** does the rack contain an element named 'name'? */
	bool hasElementNamed(const std::string& name) const;


	/** set the refresh interval (in milliseconds) */
	void setRefreshInterval(unsigned int ms);

	/** get the refresh interval (in milliseconds) */
	unsigned int getRefreshInterval() const;



	/** get a list of all NoteDevices within the Rack */
	std::vector<NoteDevice*> getNoteDevices() const;

	/** get a vector of all RackElements */
	const std::vector<RackElement*>& getElements() const;

	/** get a list of all SoundBase devices within the Rack */
	std::vector<SoundBase*> getSoundBaseDevices() const;

private:

	Context& ctx;

	/** the GUI widget representing the rack */
	RackWidget& rw;

	/** all rack elements */
	std::vector<RackElement*> elems;

	/** lock for update / deletion */
	std::mutex mutex;

	/** update running? */
	bool running;

	/** refresh enabled? */
	bool refreshing;

	/** the refreshing thread */
	std::thread* thread;

	/** the refresh interval */
	unsigned int refreshMS;


	/** refresh all gui elements */
	void run();


signals:

	/** a new entry has been added to the rack */
	//void entryAdded(int i);

	/** an entry has been removed from the rack */
	//void entryRemoved(int i);

	/** the entry at index i has been changed */
	//void entryChanged(int i);

	/** the racks content changed / elements have been edited */
	void onRackChanged();

};

#endif // RACK_H
