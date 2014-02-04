#ifndef SYNTH1_H
#define SYNTH1_H

#include <QWidget>
#include <KSynth/synth/SimpleSynth.h>
#include "RackElement.h"

class Knob;
class Slider1;
class CheckBox;
class QSpinBox;
class ImgSpin;
class VUMeter;
class PinConnector;
class TextLabel;

class Synth1 : public RackElement, public SimpleSynth {
	Q_OBJECT

public:

	/** ctor */
	explicit Synth1(Context& ctx, QWidget *parent = 0);

	void refresh() override;


protected:

	void paintEvent (QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;


private:

	struct {

		Knob* kFilterFreq;
		Knob* kFilterRes;

		Slider1* sldVolume;

		Knob* kMix;
		Knob* kFM;

		struct {
			Slider1* sA;
			Slider1* sD;
			Slider1* sS;
			Slider1* sR;
		} oscHuell;

		struct {
			Slider1* sA;
			Slider1* sD;
			Slider1* sS;
			Slider1* sR;
		} filterHuell;

		/** oscillator settings */
		struct GOSC {
			ImgSpin* spnMode;
			Knob* kOctave;
			Knob* kSemi;
			Knob* kFineOffset;
			CheckBox* chkKey;
			Knob* kPhase;
		};
		GOSC gosc[2];


		struct GLFO {
			ImgSpin* spnMode;
			Knob* kFreq;
			Knob* kAmount;
		};
		GLFO glfo[2];


		CheckBox* cRingMod;

		VUMeter* vuMeter;

		PinConnector* con;
		TextLabel* label;


	} elements;


signals:

public slots:

private slots:

	/** called when a ui element is changed */
	void onParamChange();



};

#endif // SYNTH1_H
