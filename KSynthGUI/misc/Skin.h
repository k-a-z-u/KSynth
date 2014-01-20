#ifndef SKIN_H
#define SKIN_H

#include <QImage>
#include <QIcon>
#include <QCursor>
class Context;
class MainWin;

/**
 * a little helper class to check the correct "installation"
 * of a skin for the GUI, providing a download if the
 * skin is missing
 */
class Skin {

public:


	/** load the given image from an appropriate source (resource, filesystem, ...) */
	static QImage getImage(const std::string& file, const std::string& type = "PNG");

	/** load the given icon from an appropriate source (resource, filesystem, ...) */
	static QIcon getIcon(const std::string& file, const std::string& type = "PNG");

	/** load the given cursor from an appropriate source (resource, filesystem, ...) */
	static QCursor getCursor(const std::string& file, const std::string& type = "PNG", int hotX = -1, int hotY = -1);


	/** check whether a valid skin folder exists */
	static void checkPresent(Context& ctx, MainWin& mw);

private:

	static void download(Context& ctx, MainWin& mw);


};

#endif // SKIN_H
