#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include "MidiUI.h"

class ColorButton : public MidiUI {
	Q_OBJECT

public:

	/** ctor */
	explicit ColorButton(QWidget *parent = 0);

	ParamValue getValueAsParam() const override;

	void setValueFromParam(ParamValue val) override;

	/** set the control's color */
	void setColor(QColor c);

	/** get the last mouse event (e.g. to check whether "shift" is pressed) */
	QMouseEvent* getLastEvent();

	/** render a bigger version? */
	void setBig(bool big);

protected:

	void paintEvent (QPaintEvent* event) override;

	void mouseReleaseEvent (QMouseEvent* e) override;
	void mousePressEvent (QMouseEvent* e) override;

private:

	/** the background color */
	QColor bgColor;

	/** mouse down? */
	bool isDown;

	/** last mouse event (e.g. to check whether shift is pressed) */
	QMouseEvent* lastEvt;

	/** render a bigger version? */
	bool big;

signals:

	/** checked state changed */
	void onChange();

public slots:

};

#endif // COLORBUTTON_H
