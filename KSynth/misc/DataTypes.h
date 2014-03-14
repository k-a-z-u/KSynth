/*
 * DataTypes.h
 *
 *  Created on: Oct 23, 2013
 *      Author: kazu
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include "Functions.h"
#include <cmath>

/** the amplitude between [-1;+1] */
typedef float Amplitude;

/** the sample rate in Hz */
typedef unsigned int SampleRate;

/** one sample frame (e.g. stereo) within the sample-rate */
typedef unsigned int SampleFrame;

/** describes number of channels / selects a channel */
typedef unsigned int Channel;

/** the time in seconds */
typedef float Time;

/** "time" in multiples of 128th notes */
typedef unsigned int TimeBase128;

/** "time" in multiples of 4th notes */
typedef unsigned int TimeBase4;

/** same as TimeBase128 but also allows negative numbers */
typedef int TimeOffset128;

/** the volume between [0;1] */
typedef float Volume;

/** a frequency in Hz */
typedef float Frequency;

/** the beat (in number of 128th notes) */
typedef unsigned int Beat;



/** a parameter identified by it's index */
typedef unsigned int Param;

/** value for a parameter. must be within [0,1] */
struct ParamValue {

	ParamValue(float val) : val(val) {;}
	ParamValue(int min, int max, int val) : val( float(val-min) / float(max-min) ) {;}
	ParamValue(unsigned int min, unsigned int max, unsigned int val) : val( float(val-min) / float(max-min) ) {;}
	ParamValue(float min, float max, float val) : val( (val-min) / (max-min) ) {;}
	ParamValue(bool en) : val(en?1:0) {;}

	operator float()					{return val;}
	bool asBool()						{return val >= 0.5;}
	int asInt(int min, int max)			{return min + (int) std::round( float(max-min) * val);}
	unsigned int asUInt(unsigned int min, unsigned int max) {return min + (unsigned int) std::round( float(max-min) * val);}
	float asFloat(float min, float max)	{return min + (max-min) * val;}

private:
	float val;
};
//typedef float ParamValue;


/** audio-data for several channels */
struct AudioData {

	/** left audio channel's value */
	Amplitude left;

	/** right audio channel's value */
	Amplitude right;


	AudioData() : left(0), right(0) {;}
	AudioData(Amplitude mono) : left(mono), right(mono) {;}
	AudioData(Amplitude l, Amplitude r) : left(l), right(r) {;}
	AudioData& operator *= (Volume v) {left *= v; right *= v; return *this;}
	AudioData  operator *  (Volume v) const {return AudioData(left*v, right*v);}
	AudioData& operator += (AudioData a) {left += a.left; right += a.right; return *this;}
	AudioData  operator +  (AudioData a) const {return AudioData(left+a.left, right+a.right);}
	AudioData& operator -= (AudioData a) {left -= a.left; right -= a.right; return *this;}
	AudioData  operator -  (AudioData a) const {return AudioData(left-a.left, right-a.right);}

	void clamp() {
		left = clamp1(left);
		right = clamp1(right);
	}
};

/** constant for PI */
#define K_PI		3.14159265358979323846

/** constant for 2*PI */
#define K_PI2		(2*K_PI)



#endif /* DATATYPES_H_ */
