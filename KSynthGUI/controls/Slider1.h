#ifndef SLIDER1_H
#define SLIDER1_H

#include "MidiUI.h"
#include "Snapper.h"

class Slider1 : public MidiUI {
	Q_OBJECT

public:

	/** ctor */
	explicit Slider1(QWidget *parent = 0);

	/** convenience ctor with set-up values */
	explicit Slider1(const std::string& title, int min = 0, int max = 100, int value = 0, unsigned int mouseWheelSteps = 5, QWidget *parent = 0);


	/** get the slider's current value */
	int getValue();

	/** set the slider's current value */
	void setValue(int value);

	/** set the slider's title */
	void setTitle(const std::string& title);


	ParamValue getValueAsParam() const override;

	void setValueFromParam(ParamValue val) override;

	/** add one snapping value */
	void addSnap(int val, unsigned int size);

	/** the number of steps the controll will move when the mouse-wheel is used */
	void setMouseWheelSteps(unsigned int steps);


protected:

	void paintEvent (QPaintEvent* event) override;

	void mouseReleaseEvent (QMouseEvent* e) override;
	void mousePressEvent (QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;

	void wheelEvent(QWheelEvent *event) override;


private:

	void recalc(int x, int y);

	/** the number of steps the controll will move when the mouse-wheel is used */
	unsigned int mouseWheelSteps;

	/** the slider's value */
	struct {
		int value;
		int min;
		int max;
	} value;

	/** sizeing information */
	struct {
		int w;
		int h;
		int way;			// the slider's way
		int uBorder;		// upper border between track and slider
		int lBorder;		// lower border between track and slider
		int sldTop;			// pixel from sliders top to middle
		int trackL;			// track inset from the left
	} size;

	/** the slider's title */
	std::string title;

	/** snapping values */
	Snapper<int> snapper;

signals:

	/** slider's value has changed */
	void onChange();


public slots:

};

#endif // SLIDER1_H
