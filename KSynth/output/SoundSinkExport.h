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

	/** set the file to export to */
	virtual void setFile(const K::File& file) = 0;

};

#endif // SOUNDSINKEXPORT_H
