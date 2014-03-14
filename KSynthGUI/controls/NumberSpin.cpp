#include "NumberSpin.h"
#include "../misc/Helper.h"

NumberSpin::NumberSpin(QWidget* parent, int min, int max, int val) :
	ImgSpin(parent) {

	values.min = min;
	values.max = max;

	updateImages();
	setValue(val);

}

void NumberSpin::setMinimum(int min) {
	bool changed = values.min != min;
	values.min = min;
	if (changed) {
		updateImages();
	}
}

void NumberSpin::setMaximum(int max) {
	bool changed = values.max != max;
	values.max = max;
	if (changed) {
		updateImages();
	}
}

void NumberSpin::setValue(int val) {
	bool changed = values.val != val;
	values.val = val;
	if (changed) {
		setSelectedIndex(values.val - values.min);
	}
}

int NumberSpin::getValue() const {
	return values.val;
}

void NumberSpin::updateImages() {
	clear();
	for (int i = values.min; i <= values.max; ++i) {
		add( Helper::getForNumber(i) );
	}
	emit repaint();
}
