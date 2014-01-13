#ifndef REVERB1_H
#define REVERB1_H

#include "RackElement.h"
#include <KSynth/fx/Reverb.h>

class Knob;

class Reverb1 : public RackElement, public Reverb {
	Q_OBJECT

public:

	/** ctor */
	explicit Reverb1(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~Reverb1();

	//std::string getDeviceType() const override;

	//RackElement* createFromPrototype(Context& ctx) const override;

	void refresh() override;

protected:

	void paintEvent (QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;

	struct {

		Knob* kDry;
		Knob* kNum;
		Knob* kDelay;

	} elements;



signals:

public slots:

	void onValueChanged();

};

#endif // REVERB1_H
