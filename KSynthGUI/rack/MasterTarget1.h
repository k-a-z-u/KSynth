#ifndef MASTERTARGET1_H
#define MASTERTARGET1_H

#include "RackElement.h"
#include <KSynth/output/MasterTarget.h>
#include <KSynth/misc/Units.h>
//#include <KLib/math/dsp/DFT.h>

class VUMeter;
class FFTAnalyzer;
class PinConnector;
#include <QPainter>
#include <QVector>
#include <iostream>
class FFTLCD : public QWidget {

public:

	FFTLCD() {
		setMinimumSize(138, 26);
		setMaximumSize(138, 26);
		paint.dashes << 0.5f << 0.5f;
		paint.pen.setColor(QColor(255,255,255,180));
		paint.pen.setDashPattern(paint.dashes);
		paint.pen.setWidth(2);
	}

	void setValues(std::vector<float> next) {
		values.resize(next.size());
		for (unsigned int i = 0; i < next.size(); ++i) {

			//if (next[i] != next[i]) {continue;}

			float percent = float(i) / float(next.size());
			float scale = float(1.0 + std::pow(percent, 1.5f));
			float v = next[i] * scale;

			v = -std::log(next[i]);
			if (v > 10) {v = 10;}
			v = 10 - v;
			if (v >= values[i]) {values[i] = v;} else {values[i] *= 0.75f;}

		}
		emit repaint();
	}

protected:


	void paintEvent(QPaintEvent* event) override {

		if (values.empty()) {return;}

		Q_UNUSED(event);
		QPainter p(this);

		p.setPen(paint.pen);

		// find max
		float max = 16;

		// render
		float pStep = 3.0f / float(width());
		unsigned int x = 0;
		int h = height();

		for (float percent = 0; percent <= 1.0; percent += pStep) {

			// rescale
			//float pScale = std::pow( (percent*0.7+0.3), 5);
			float pScale = float( std::pow(percent, 1.8) );
			unsigned int idx = (unsigned int) (float(values.size()) * pScale);

			float v = values.at(idx);
			//if (v < 0 || v > 100) {continue;}
			int y1 = h - int( float(h) * v / max * 0.95);
			p.drawLine(x-1, h, x-1, y1);
			x += 3;
		}

	}

private:

	std::vector<float> values;

	struct {
		QVector<qreal> dashes;
		QPen pen;
	} paint;


};


class MasterTarget1 : public RackElement, public MasterTarget {
	Q_OBJECT

private:

	struct {

		VUMeter* vuLeft;
		VUMeter* vuRight;

		PinConnector* connector;

	} elements;


	/** perform pcm -> fft */
	FFTAnalyzer* fft;

	/** render fft into a LCD */
	FFTLCD lcd;

public:

	/** ctor */
	explicit MasterTarget1(Context& ctx, QWidget *parent = 0);

	/** dtor */
	virtual ~MasterTarget1();

	void refresh() override;

	//RackElement* createFromPrototype(Context& ctx) const override;

	void process(Amplitude** inputs, Amplitude** outputs) override;


	void onRackAttach() override;

	void onRackDetach() override;




protected:

	void paintEvent(QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;



signals:

public slots:

};

#endif // MASTERTARGET1_H
