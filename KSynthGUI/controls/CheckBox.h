#ifndef CHECKBOX_H
#define CHECKBOX_H

class CheckBoxGroup;
#include "MidiUI.h"

class CheckBox : public MidiUI {
	Q_OBJECT

public:

	explicit CheckBox(QWidget *parent = 0);

	explicit CheckBox(const std::string& title, bool checked = false, QWidget *parent = 0);

	/** is the control currently checked? */
	bool isChecked() const;

	/** make the control checked/unchecked */
	void setChecked(bool checked);

	/** set the checkbox's title */
	void setTitle(const std::string& title);

	ParamValue getValueAsParam() const override;

	void setValueFromParam(ParamValue val) override;

protected:

	void paintEvent (QPaintEvent* event) override;

	void mousePressEvent (QMouseEvent* e) override;

private:

	/** check box's title (if any) */
	std::string title;

	/** current state */
	bool checked;

signals:

	friend class CheckBoxGroup;

	/** checked state changed */
	void onChange();

public slots:

};

#endif // CHECKBOX_H
