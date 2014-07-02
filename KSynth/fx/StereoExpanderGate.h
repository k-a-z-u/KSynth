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
		bqLeft.setHighShelf(0.35f, 1.0f, 1.0f*w);
		bqRight.setLowShelf(0.35f, 1.0f, 1.0f/w);
		delay.setDelay( (int) (48.0f*w) );
	}

	float getWidth() const {
		return width;
	}

	void process(Amplitude in, Amplitude& o1, Amplitude& o2) {
		Amplitude l = bqLeft.filter(0, in);
		Amplitude r = bqRight.filter(0, in);
		delay.push(r);
		r = delay.pop();
		Amplitude l1 = l + (-r) * 0.15f;
		Amplitude r1 = r + (-l) * 0.15f;
		o1 = l1 + (-r1) * 0.15f;
		o2 = r1 + (-l1) * 0.15f;
	}


private:

	float width;
	BiquadFilterGate<1> bqLeft;
	BiquadFilterGate<1> bqRight;
	DelayBufferFixed<float, 1024> delay;

};

#endif // STEREOEXPANDERGATE_H
