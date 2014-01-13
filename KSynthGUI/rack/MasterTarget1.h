#ifndef MASTERTARGET1_H
#define MASTERTARGET1_H

#include "RackElement.h"
#include <KSynth/output/MasterTarget.h>
#include <KSynth/misc/Units.h>

class VUMeter;
class FFTAnalyzer;
class PinConnector;
#include <QPainter>
#include <QVector>

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

			float percent = float(i) / float(next.size());
			float scale = std::pow(percent+0.2f, 2.5f);
			float v = next[i] * scale;

			if (v >= values[i]) {values[i] = v;} else {values[i] *= 0.7f;}

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
		//static float max = 0.001f;
		//for (float d : values) {if (d > max) {max = d;}}
		float max = 4;

		// render
		float pStep = 3.0f / float(width());
		unsigned int x = 0;
		unsigned int h = height();

		for (float percent = 0; percent <= 1.0; percent += pStep) {

			// rescale
			float pScale = float( std::pow(percent, 2) );
			int idx = int ( float(values.size()) * pScale);

			float v = values[idx];
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

	FFTAnalyzer* fft;

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
