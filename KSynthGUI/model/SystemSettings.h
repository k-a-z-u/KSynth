#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

class SoundSinkHardware;
class Context;

#include <string>

namespace K {
	class File;
}

/**
 * all configurable system settings reside here
 */
class SystemSettings {

public:

	/** set the sound sink to use for audio output */
	void setSoundSink(SoundSinkHardware* sink);

	/** get the currently used sound sink */
	SoundSinkHardware* getSoundSink() const;


	/** set the refresh interval for the gui (in milliseconds) */
	void setGuiRefreshInterval(unsigned int ms);

	/** get the refresh interval for the gui (in milliseconds) */
	unsigned int getGuiRefreshInterval() const;


	/** load settings from file. returns a list of problems */
	std::string load(const K::File& file);

	/** save settings to file */
	void save(const K::File& file);



private:

	/** the main context */
	Context& ctx;


private:

	friend class Context;

	/** ctor */
	SystemSettings(Context& ctx);

	/** hidden copy ctor */
	SystemSettings(const SystemSettings&);

	/** hidden assignment operator */
	SystemSettings& operator = (const SystemSettings&);


};

#endif // SYSTEMSETTINGS_H
