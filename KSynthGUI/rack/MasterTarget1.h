#ifndef MASTERTARGET1_H
#define MASTERTARGET1_H

#include "RackElement.h"
#include <KSynth/output/MasterTarget.h>
#include <KSynth/misc/Units.h>
//#include <KLib/math/dsp/DFT.h>

class VUMeter;
class FFTAnalyzer;
class PinConnector;
class LCD;
#include <iostream>



/** provides falloff and scaling for frequency spectrum */
class FFTFalloff {

public:

	FFTFalloff(unsigned int numVals) : numVals(numVals) {
		cur.resize(numVals);
	}

	/** get FFT array idx by given percentage using a "logarithmic" frequency scale */
	unsigned int percentToIdx(float percent, int fftSize) {
		int min = int(float(fftSize) * 100.0f / 48000.0f);
		int max = int(float(fftSize) * 30000.0f / 48000.0f);
		percent = std::pow(percent, 1.8f);
		return (unsigned int) ( min + int( float(max-min) * percent ) );
	}

	/** get the current values */
	void setValues( std::vector<float> next ) {

		unsigned int maxIdx = (unsigned int) next.size();

		// scale values. increase values at the end of the spectrum.
		for (unsigned int i = 0; i < next.size(); ++i) {

			float percent = float(i) / float(next.size());
			float scale = float(0.5 + percent*2);
			float v = next[i] * scale;
			if (v > 16) {v = 16;}
			next[i] = v;

		}

		// combine values to some bands
		for (unsigned int i = 0; i < numVals; ++i) {

			float percent1 = float(i+1) / float(numVals+1);
			float percent2 = float(i+2) / float(numVals+1);

			// rescale frequencies to show more low and less high frequencies
			unsigned int idx0 = percentToIdx(percent1, (int) next.size());//2 + (unsigned int) (float(next.size()-2) * float( std::pow(percent1, 1.8) ));
			unsigned int idx1 = percentToIdx(percent2, (int) next.size())-1;//2 + (unsigned int) (float(next.size()-2) * float( std::pow(percent2, 1.8) )) - 1;

			if (idx1 <= idx0) {idx1 = idx0+1;}
			if (idx1 >= maxIdx) {idx1 = maxIdx-1;}


			// get max value for this band
			float v = 0;
			for (unsigned int i = idx0; i < idx1; ++i) {
				float tmp = next.at(i);
				if (tmp > v) {v = tmp;}
			}

			// map to range of [0;1]
			v /= 16;

			// store the value or falloff
			if (cur[i] > v) {cur[i] *= 0.9f;} else {cur[i] = v;}

		}


	}

	/** get the modified values */
	const std::vector<float>& getValues() const {return cur;}

private:

	unsigned int numVals;
	std::vector<float> cur;

};


class MasterTarget1 : public RackElement, public MasterTarget {
	Q_OBJECT

private:

	/** all UI elements */
	struct {
		VUMeter* vuLeft;
		VUMeter* vuRight;
		PinConnector* connector;
	} elements;


	/** perform pcm -> fft */
	FFTAnalyzer* fft;

	/** render fft into a LCD */
	FFTFalloff fftFalloff;
	LCD* lcd;

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
