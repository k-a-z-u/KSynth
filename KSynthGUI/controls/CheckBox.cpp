#include "CheckBox.h"
#include "../misc/Helper.h"

CheckBox::CheckBox(QWidget *parent) :
	MidiUI(parent), checked(false) {

	setTitle("");

}

CheckBox::CheckBox(const std::string& title, bool checked, QWidget *parent) :
	CheckBox(parent) {
	setTitle(title);
	this->checked = checked;
}

bool CheckBox::isChecked() const {
	return this->checked;
}

void CheckBox::setChecked(bool checked) {
	bool changed = this->checked != checked;
	this->checked = checked;
	if (changed) {emit repaint();}
}

ParamValue CheckBox::getValueAsParam() const {
	return ParamValue(isChecked());
}

void CheckBox::setValueFromParam(ParamValue val) {
	setChecked(val.asBool());
}

void CheckBox::setTitle(const std::string& title) {
	this->title = title;
	unsigned int w = (unsigned int) title.length() * 6;
	if (!title.empty()) {w+=4;}
	setMinimumSize(16+w,16);
	setMaximumSize(16+w,16);
}


void CheckBox::mousePressEvent (QMouseEvent* e) {
	Q_UNUSED(e);
	setChecked(!checked);
	emit onChange();
}

#include <QPainter>
void CheckBox::paintEvent (QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgUnchecked = Helper::getSkinImage("skin/chkSmlOff.png", "PNG");
	static QImage imgChecked = Helper::getSkinImage("skin/chkSmlOn.png", "PNG");

	static QColor cText(255,255,255);
	static QFont font; font.setPixelSize(7); font.setBold(true);

	if (checked) {
		p.drawImage(0,0,imgChecked);
	} else {
		p.drawImage(0,0,imgUnchecked);
	}

	if (!title.empty()) {
		p.setFont(font);
		p.setPen(cText);
		p.drawText(16+2,8+3, title.c_str());
	}

}
