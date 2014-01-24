#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

class SoundSink;
class Context;

/**
 * all configurable system settings reside here
 */
class SystemSettings {

public:

	/** set the sound sink to use for audio output */
	void setSoundSink(SoundSink* sink);

	/** get the currently used sound sink */
	SoundSink* getSoundSink() const;


	/** set the refresh interval for the gui (in milliseconds) */
	void setGuiRefreshInterval(unsigned int ms);

	/** get the refresh interval for the gui (in milliseconds) */
	unsigned int getGuiRefreshInterval() const;


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
