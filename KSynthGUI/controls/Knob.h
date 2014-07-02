#ifndef KNOB_H
#define KNOB_H

#include "MidiUI.h"
#include "Snapper.h"
#include <vector>

class Knob : public MidiUI {
	Q_OBJECT

public:

	/** ctor */
	explicit Knob(QWidget* parent = 0);

	/** ctor with title */
	explicit Knob(const std::string& title, int min = 0, int max = 100, int value = 0, unsigned int mouseWheelSteps = 5, QWidget* parent = 0);


	/** set the knob's value */
	int getValue() const;

	/** get the knob's current value */
	void setValue(int value);



	ParamValue getValueAsParam() const override;

	void setValueFromParam(ParamValue val) override;


	/** set the knob's title */
	void setTitle(const std::string& str);

	/** add a new snapping point */
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

	/** set the knob's value */
	void _setValue(int v);


signals:

	/** the knob's value changed */
	void onChange();


private:

	/** track mouse-state */
	struct MS {
		int x;
		int y;
		bool isDown;
		int downValue;
		MS() : x(0), y(0), isDown(0), downValue(0) {;}
	} mouseState;

	/** the knob's value */
	struct V {
		int value;
		int min;
		int max;
		V() : value(0), min(0), max(0) {;}
	} value;

	/** the number of steps the controll will move when the mouse-wheel is used */
	unsigned int mouseWheelSteps;

	/** the knob's title */
	std::string title;

	/** spannings */
	Snapper<int> snapper;

};

#endif // KNOB_H
