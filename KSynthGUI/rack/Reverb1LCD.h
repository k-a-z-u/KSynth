#ifndef REVERB1LCD_H
#define REVERB1LCD_H

#include <QWidget>

class Reverb1LCD : public QWidget {

public:

	Reverb1LCD(QWidget* parent = 0);

	/** draw an impulse response using the given data */
	void drawImpulseResponse(const float* data, unsigned int size);

	/** draw the given values adjusted to the LCDs size by skipping values */
	void drawValues(const float* data, unsigned int size);

protected:

	void paintEvent(QPaintEvent* event) override;

private:

	/** adjust the image buffer to match the control's size */
	void resizeImageBuffer();

	/** an image buffer */
	QImage img;

};

#endif // REVERB1LCD_H
