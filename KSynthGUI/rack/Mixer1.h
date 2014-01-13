#ifndef MIXER1_H
#define MIXER1_H

#include <QWidget>
#include <KSynth/mixer/SimpleMixer.h>
#include "RackElement.h"

class Knob;
class Slider1;
class VUMeter;
class Knob;
class LED1;
class PinConnector;


class Mixer1 : public RackElement, public SimpleMixer {
	Q_OBJECT

public:

	/** ctor */
	explicit Mixer1(Context& ctx, QWidget *parent = 0);

	//std::string getDeviceType() const override;

	//RackElement* createFromPrototype(Context& ctx) const override;

//	virtual std::string getProductString() const {
//		return SimpleMixer::getProductString();
//	}

protected:

	void refresh() override;

	void paintEvent(QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;


private:

	/** one slot (input) of the mixer */
	struct GSlot {
		Slider1* sldVolume;
		Knob* kHighPass;
		Knob* kLowPass;
		Knob* kPan;
		LED1* ledLowPass;
		LED1* ledHighPass;
		VUMeter* vuMeter;
	};

	struct {

		/** settings for each slot */
		struct GSlot gslots[9];

		/** the main VU meter */
		VUMeter* vuMeter;

		/** the main volume */
		Slider1* sldMainVol;

		PinConnector* connector;

	} elements;


private slots:

	/** called when a ui element is changed */
	void onParamChange();

signals:

public slots:

};

#endif // MIXER1_H
