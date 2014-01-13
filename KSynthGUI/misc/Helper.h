#ifndef HELPER_H
#define HELPER_H

#include <QImage>
#include <KSynth/oscillator/SimpleOscillator2.h>

class Helper {

public:

	/** load the given image from an appropriate source (resource, filesystem, ...) */
	static QImage getSkinImage(const std::string& file, const std::string& type = "PNG");

	/** get QImage for the given oscillator function */
	static QImage getForOscFunction(SimpleOscillator2Mode mode, unsigned int w, unsigned int h);

	/** get QImage for the given number */
	static QImage getForNumber(int number, unsigned int w, unsigned int h);


	/** add a LED look */
	static void LEDify(QImage& img);


};

#endif // HELPER_H
