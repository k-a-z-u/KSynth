#ifndef SAMPLEPAD1_H
#define SAMPLEPAD1_H

#include <KSynth/sampler/SimpleSamplePad.h>
#include "RackElement.h"

class ColorButton;

class SamplePad1 : public RackElement, public SimpleSamplePad {
	Q_OBJECT

public:

	/** ctor */
	explicit SamplePad1(Context& ctx, QWidget *parent = 0);


protected:

	void refresh() override;

	void paintEvent(QPaintEvent*) override;

	void resizeEvent(QResizeEvent* event) override;


private slots:

	void onSampleClick();


private:

	struct {

		ColorButton* cbSample[SSP_SAMPLES];

	} elements;


};

#endif // SAMPLEPAD1_H
