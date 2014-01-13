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

protected:

	void paintEvent (QPaintEvent* event) override;

	void mouseReleaseEvent (QMouseEvent* e) override;
	void mousePressEvent (QMouseEvent* e) override;

private:

	/** the background color */
	QColor bgColor;

	/** mouse down? */
	bool isDown;

signals:

	/** checked state changed */
	void onChange();

public slots:

};

#endif // COLORBUTTON_H
