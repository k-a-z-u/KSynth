#ifndef VUMETER_H
#define VUMETER_H

#include <QWidget>

enum class VUMeterOrientation {
	HORIZONTAL,
	VERTICAL
};

class VUMeter : public QWidget {
	Q_OBJECT

public:

	explicit VUMeter(QWidget *parent = 0);

	/** set the VU value */
	void setValue(float value);

	/** get the VU value */
	float getValue() const;

	/** set the VU-meters orientation */
	void setOrientation(VUMeterOrientation orientation);

protected:

	void paintEvent (QPaintEvent* event) override;


private:

	/** the current value to display */
	float value;

	/** the last repaint() call. used for falloff */
	uint64_t lastMS;

	/** the VU-meters orientation */
	VUMeterOrientation orientation;


signals:

public slots:



};

#endif // VUMETER_H
