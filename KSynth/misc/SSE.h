/*
 * SSE.h
 *
 *  Created on: Nov 7, 2013
 *      Author: kazu
 */

#ifndef SSE_H_
#define SSE_H_

#include <xmmintrin.h>

/** align at 16 byte boundaries */
#define ALIGN16		__attribute__((aligned (16)))


/** union for 4 floating-point values */
union SSEfloat {

	__m128	sse;

	float	arr[4];

	struct {
		float v1;
		float v2;
		float v3;
		float v4;
	};

	/** empty ctor */
	SSEfloat() {;}

	/** ctor from other value */
	SSEfloat(__m128 val) : sse(val) {;}

	/** ctor from float. assign to all elements */
	SSEfloat(float val) {
		this->sse = _mm_set_ps1(val);
	}

	/** ctor from 4 float values */
	SSEfloat(float v1, float v2, float v3, float v4) {
		this->sse = _mm_set_ps(v4, v3, v2, v1);
	}



	/** assign float to all elements */
	SSEfloat& operator = (float val) {
		this->sse = _mm_set_ps1(val);
		return *this;
	}

	SSEfloat& operator *= (const SSEfloat& other) {
		this->sse = _mm_mul_ps(this->sse, other.sse);
		return *this;
	}
	SSEfloat operator * (const SSEfloat& other) const {
		return SSEfloat(_mm_mul_ps(this->sse, other.sse));
	}

	SSEfloat& operator += (const SSEfloat& other) {
		this->sse = _mm_add_ps(this->sse, other.sse);
		return *this;
	}

};

/*
SSE a = {1,2,3,4};
SSE b = {5,6,7,8};
SSE c = {0};

c.sse = _mm_mul_ps(a.sse, b.sse);
float sum = _mm_cvtss_f32(c.sse);
std::cout << c.elem.a << ":" << c.elem.b << ":" << c.elem.c << ":" << c.elem.d << std::endl;
std::cout << sum << std::endl;
 */

#endif /* SSE_H_ */
