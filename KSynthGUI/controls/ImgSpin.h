#ifndef IMGSPIN_H
#define IMGSPIN_H

class QPushButton;

#include <vector>
#include "MidiUI.h"

class ImgSpin : public MidiUI {
	Q_OBJECT

public:

	explicit ImgSpin(QWidget *parent = 0);

	/** remove all entries from the spinner */
	void clear();

	/** add a new entry */
	void add(const QImage& img, int value = 0);

	/** get the currently selected index */
	unsigned int getSelectedIndex();

	/** set the selected index */
	void setSelectedIndex(unsigned int idx);

	/** get the value behind the selected entry (if any) */
	int getSelectedValue();


	ParamValue getValueAsParam() const override;

	void setValueFromParam(ParamValue val) override;

protected:

	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:

	QPushButton* btnNext;
	QPushButton* btnPrev;

	struct Entry {
		QImage img;
		int value;
		Entry(const QImage& img, int value) : img(img), value(value) {;}
	};

	std::vector<Entry> entries;

	unsigned int curIdx;

private slots:

	/** next has been pressed */
	void onNext();

	/** prev has been pressed */
	void onPrev();

signals:

	/** the currently selected item has changed */
	void onChange();


public slots:


};

#endif // IMGSPIN_H
