#ifndef REVERB1_H
#define REVERB1_H

#include "RackElement.h"
#include <KSynth/fx/AllPassReverb.h>

class Knob;
class LCD;
class QPushButton;
class PinConnector;

#include <KLib/math/dsp/dft/FFT.h>

class Reverb1 : public RackElement, public AllPassReverb {
	Q_OBJECT

public:

	/** ctor */
	explicit Reverb1(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~Reverb1();

	void refresh() override;

protected:

	void paintEvent (QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;

	struct Gate {
		Knob* kDelay;
		Knob* kPosGain;
		Knob* kNegGain;
	};

	struct {
		Gate gates[4];
		Knob* kDryWet;
	} elements;

	PinConnector* connector;

	QPushButton* btn;

	/** LCD impulse-response / frequency update */
	struct Analysis {

		LCD* lcd;

		std::vector<float> vec1;
		std::vector<float> vec2;
		std::vector<float> vec3;

		K::FFT fft;

		bool needsUpdate;

		unsigned int size;

		unsigned int mode;

		/** ctor */
		Analysis() : fft(65536), needsUpdate(false), size(65536), mode(0) {
			vec1.resize(65536);
			vec2.resize(65536);
			vec3.resize(96);
		}

	} analysis;

private:

	void showImpulseResponse();

signals:

private slots:

	/** called when a ui element is changed */
	void onParamChange();

	void onToggleLCDmode();

};

#endif // REVERB1_H
