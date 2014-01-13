#ifndef DRUMCOMPUTER1_H
#define DRUMCOMPUTER1_H

#include "RackElement.h"
#include <KSynth/sampler/SimpleDrumComputer.h>

class CheckBoxGroup;
class CheckBox;
class ColorButton;
class Knob;
class VUMeter;
class Slider1;
class LED1;

class QPushButton;
class QLabel;
class PinConnector;

class DrumComputer1 : public RackElement, public SimpleDrumComputer {
	Q_OBJECT

public:

	/**  ctor */
	explicit DrumComputer1(Context& ctx, QWidget* parent = 0);

	/** dtor */
	~DrumComputer1();

	void onRackAttach() override;

	void onRackDetach() override;


protected:

	void refresh() override;

	void paintEvent(QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;


private slots:

	/** select the currently active pattern */
	void selectPattern();

	/** select the to-be-edited bank */
	void selectBank();

	/** select / unselect a beat within the currently selected bank / pattern */
	void selectBeat();

	void onParamChange();

	/** open sample for a slot. caller needs to set a property named IDX (between [0,7]) */
	void openSample();

private:

	struct {

		/** select the currently active pattern (all banks) */
		CheckBox* chkPattern[16];
		CheckBoxGroup* grpPattern;

		/** elements for each bank */
		struct Bank {
			QPushButton* btnOpen;
			QLabel* lblFile;
			CheckBox* chkEdit;
			Knob* kVol;
			LED1* ledBeat;
		};
		Bank banks[8];
		CheckBoxGroup* grpBanks;

		/** select the active beats for a bank+pattern */
		ColorButton* chkBeat[16*2];

		/** vu display */
		VUMeter* vu;

		/** main-volume */
		Slider1* sldVol;


		/** memory usage */
		struct Memory {

			unsigned int used;
			QImage img;
			QLabel* lbl;

			/** ctor */
			Memory() : used(0), img(), lbl(nullptr) {;}

		} memory;

		PinConnector* connector;


	} elements;




	/** the currently to-be-edited bank */
	unsigned int curBank;

};

#endif // DRUMCOMPUTER1_H
