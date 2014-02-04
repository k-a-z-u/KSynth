#ifndef SOUNDSINKEXPORT_H
#define SOUNDSINKEXPORT_H

#include "SoundSink.h"
#include <KLib/fs/File.h>

/**
 * export SoundSinks write piped audio data
 * to files on the disk.
 *
 * those sinks provide additional methods for
 * setting the output file, etc.
 */
class SoundSinkExport : public SoundSink {

public:

	/** dtor */
	virtual ~SoundSinkExport() {;}

	/** set the file to export to */
	virtual void setFile(const K::File& file) = 0;

	/**
	 * get the extension for the resulting export-files.
	 * without the leading ".". e.g. "wav"
	 */
	virtual std::string getFileExtension() = 0;

};

#endif // SOUNDSINKEXPORT_H
