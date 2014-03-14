#ifndef NUMBERSPIN_H
#define NUMBERSPIN_H

#include "ImgSpin.h"

class NumberSpin : public ImgSpin {

public:

	/** ctor */
	explicit NumberSpin(QWidget* parent, int min=0, int max=10, int val=0);

	/** set the minimum value */
	void setMinimum(int min);

	/** set the maximum value */
	void setMaximum(int max);


	/** set the current value */
	void setValue(int val);

	/** get the current value */
	int getValue() const;

private:

	/** create images for all values between min and max */
	void updateImages();

	struct {
		int min;
		int max;
		int val;
	} values;

};

#endif // NUMBERSPIN_H
