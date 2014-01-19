#ifndef HELPER_H
#define HELPER_H

#include <QImage>
#include <QIcon>
#include <QCursor>
#include <KSynth/oscillator/SimpleOscillator2.h>

class Helper {

public:

	/** load the given image from an appropriate source (resource, filesystem, ...) */
	static QImage getSkinImage(const std::string& file, const std::string& type = "PNG");

	/** load the given icon from an appropriate source (resource, filesystem, ...) */
	static QIcon getSkinIcon(const std::string& file, const std::string& type = "PNG");

	/** load the given cursor from an appropriate source (resource, filesystem, ...) */
	static QCursor getSkinCursor(const std::string& file, const std::string& type = "PNG", int hotX = -1, int hotY = -1);


	/** get QImage for the given oscillator function */
	static QImage getForOscFunction(SimpleOscillator2Mode mode, unsigned int w, unsigned int h);

	/** get QImage for the given number */
	static QImage getForNumber(int number, unsigned int w, unsigned int h);


	/** add a LED look */
	static void LEDify(QImage& img);


};

#endif // HELPER_H
