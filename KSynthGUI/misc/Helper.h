#ifndef HELPER_H
#define HELPER_H

#include <QImage>
#include <KSynth/oscillator/SimpleOscillator2.h>

class Helper {

public:


	/** get QImage for the given oscillator function */
	static QImage getForOscFunction(SimpleOscillator2Mode mode, unsigned int w, unsigned int h);

	/** get QImage for the given number */
	static QImage getForNumber(int number, int w = -1, int h = -1);


	/** add a LED look */
	static void LEDify(QImage& img);

	/** add a LED look */
	static void LEDify2(QImage& img);

private:

	/** draw the digit "digit" at position (sx,sy) using the given painter */
	static void drawDigit(QPainter& p, char digit, int sx, int sy);

};

#endif // HELPER_H
