#ifndef CHECKBOXGROUP_H
#define CHECKBOXGROUP_H

#include "CheckBox.h"
#include <QWidget>
#include <vector>

/**
 * @brief The CheckBoxGroup class
 * combine several checkboxes to create radio-button like
 * behaviour. one box checked -> others disabled
 */
class CheckBoxGroup : public QWidget {
	Q_OBJECT

public:
	CheckBoxGroup();

	/** add a child member */
	void add(CheckBox* child);


private slots:

	/** callback */
	void onChange();


private:

	/** all entries */
	std::vector<CheckBox*> childs;

};

#endif // CHECKBOXGROUP_H
