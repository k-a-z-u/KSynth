#ifndef STEREOEXPANDERGATE_H
#define STEREOEXPANDERGATE_H

#include "BiquadFilterGate.h"
#include "../misc/DataTypes.h"
#include "DelayBufferFixed.h"

/**
 * a very simple stereo expander using phase modification
 *http://music.tutsplus.com/tutorials/mastering-elements-part-4-stereo-enhancement--audio-7057
 *http://www.soundonsound.com/sos/sep10/articles/pt-0910.htm
 *http://www.benvesco.com/blog/mixing/2008/turn-a-mono-track-into-rich-stereo/
 *
 *
 *delay: max 5 ms, sonst klingts wie echo
 *polarity invert?
 *
 */
class StereoExpanderGate {

public:

	/** ctor */
	StereoExpanderGate() {
		setWidth(2.0f);
	}

	void setWidth(float w) {
		this->width = w;
		float tmp = 1 + w * 4;
		bqLeft.setHighShelf(0.35f, 1.0f, 1.0f*tmp);
		bqRight.setLowShelf(0.35f, 1.0f, 1.0f/tmp);
		delay.setDelay( (int) (w * 40.0f) );
	}

	float getWidth() const {
		return width;
	}

	void process(Amplitude i1, Amplitude i2, Amplitude& o1, Amplitude& o2) {

		if (width < 0.01f) {

			o1 = i1;
			o2 = i2;

		} else {

			Amplitude l = bqLeft.filter(0, i1);
			Amplitude r = bqRight.filter(0, i2);
			delay.push(l);
			l = delay.pop();
			Amplitude l1 = l + (-r) * 0.15f;
			Amplitude r1 = r + (-l) * 0.15f;
			o1 = l1 + (-r1) * 0.15f;
			o2 = r1 + (-l1) * 0.15f;

		}

	}


private:

	float width;
	BiquadFilterGate<1> bqLeft;
	BiquadFilterGate<1> bqRight;
	DelayBufferFixed<float, 1024> delay;

};

#endif // STEREOEXPANDERGATE_H
