#include "SamplePad1.h"
#include "../misc/Skin.h"

#include "../controls/ColorButton.h"

SamplePad1::SamplePad1(Context& ctx, QWidget *parent) : RackElement(ctx, parent) {

	setSize(760, 128);

	for (unsigned int i = 0; i < SSP_SAMPLES; ++i) {
		elements.cbSample[i] = new ColorButton(this);
		elements.cbSample[i]->setBig(true);
		elements.cbSample[i]->connectTo(i, this, SLOT(onSampleClick()));
	}

	loadSample( K::File("/apps/workspaces/KSynth/_build/samples/samples/musicradar-techno-120bpm/Bassline U/TechBassU120G-04.wav"),  0);

}

void SamplePad1::refresh() {
	;
}

void SamplePad1::onSampleClick() {

	ColorButton* cb = (ColorButton*) sender();

	playSample(cb->getParamType(), 1.0f);

}

void SamplePad1::resizeEvent(QResizeEvent* event) {
	Q_UNUSED(event);

	int x1 = 8;
	int y1 = 8;
	int sx = 64;
	int sy = 64;

	for (unsigned int i = 0; i < SSP_SAMPLES; ++i) {
		unsigned int x = i % 8;
		unsigned int y = i / 8;
		elements.cbSample[i]->move( x1+x*sx, y1+y*sy );
	}

}


#include <QPainter>

void SamplePad1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Skin::getImage("skin/samplePad.png", "PNG");
	p.drawImage(0,0, imgBg);

}
