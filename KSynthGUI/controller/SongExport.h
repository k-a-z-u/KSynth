#ifndef SONGEXPORT_H
#define SONGEXPORT_H

class Context;

/**
 * export the current song to mp3, wave, etc..
 */
class SongExport {

public:

	/** ctor */
	SongExport(Context& ctx);

	/** show dialog to chose the export file and format */
	void showExportDialog();

private:

	Context& ctx;

};

#endif // SONGEXPORT_H
