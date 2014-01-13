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
	explicit Knob(const std::string& title, int min = 0, int max = 100, int value = 0, QWidget* parent = 0);


	/** set the knob's value */
	int getValue() const;

	/** get the knob's current value */
	void setValue(int value);



	ParamValue getValueAsParam() const override;

	void setValueFromParam(ParamValue val) override;


	/** set the knob's title */
	void setTitle(const std::string& str);

	/** add a new snapping point */
	void addSnap(int val, int size);

protected:

	void paintEvent (QPaintEvent* event) override;

	void mouseReleaseEvent (QMouseEvent* e) override;
	void mousePressEvent (QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;


signals:

	/** the knob's value changed */
	void onChange();

public slots:


private:

	struct {
		int x;
		int y;
		bool isDown;
		int downValue;
	} mouseState;

	/** the knob's value */
	struct {
		int value;
		int min;
		int max;
	} value;

	/** the knob's title */
	std::string title;

	/** spannings */
	Snapper<int> snapper;

};

#endif // KNOB_H
