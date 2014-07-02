#ifndef PHASER1_H
#define PHASER1_H

#include "RackElement.h"
#include <KSynth/fx/Phaser.h>

class Knob;
class ImgSpin;
class PinConnector;

class Phaser1 : public RackElement, public Phaser {
	Q_OBJECT

public:

	/** ctor */
	explicit Phaser1(Context& ctx, QWidget *parent = 0);

protected:

	void refresh() override;

	void paintEvent(QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;

private slots:

	/** called when a ui element is changed */
	void onParamChange();

private:

	struct {
		Knob* kMin;
		Knob* kMax;
		Knob* kSize;
		Knob* kFreq;
		Knob* kDryWet;
		ImgSpin* spnMode;
		PinConnector* connector;
	} elements;

};

#endif // PHASER1_H
