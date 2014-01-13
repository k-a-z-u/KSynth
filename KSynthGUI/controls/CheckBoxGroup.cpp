#include "CheckBoxGroup.h"

CheckBoxGroup::CheckBoxGroup() {
	;
}

void CheckBoxGroup::add(CheckBox* child) {
	childs.push_back(child);
	connect(child, SIGNAL(onChange()), this, SLOT(onChange()));
	if (childs.size() == 1) {childs[0]->setChecked(true);}
}

void CheckBoxGroup::onChange() {

	QObject* obj = QObject::sender();
	CheckBox* chk = (CheckBox*) obj;

	static bool running = false;

	if (running) {return;}
	running = true;

	// uncheck all other boxes
	if (chk->isChecked()) {
		for (CheckBox* other : childs) {
			if (other != chk) {
				bool changed = other->isChecked() != false;
				other->setChecked(false);
				if (changed) {emit other->onChange();}
			}
		}
	} else {
		chk->setChecked(true);
		emit chk->onChange();
	}

	running = false;

}
