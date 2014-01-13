#ifndef SEQUENCER1_H
#define SEQUENCER1_H

#include <KSynth/pattern/PatternSequencer.h>
#include <QMouseEvent>
#include "RackElement.h"

class QSpinBox;
class QGroupBox;
class Sequencer1Selector;
class ImgSpin;
class CheckBox;
class CheckBoxGroup;

/**
 * @brief The Sequencer1Scaler class
 * scale between gui <-> note
 */
struct Sequencer1Scaler {

	/** height of one note */
	const int noteH = 8;
	int noteW = 32;

	/** x-offset of the whole drawing */
	const int x = 2;

	/** y-offset of the whole drawing */
	const int y = 0;

	const int w = noteW*4*4;
	const int h = noteH*12;

	int oct = 2;

	int bars = 4;
	int granulartiy = 4;

	void setBars(int bars) {
		this->bars = bars;
		noteW = w / bars / granulartiy;
	}

	void setGranularity(int granularity) {
		this->granulartiy = granularity;
		noteW = w / bars / granulartiy;
	}

	void setOctave(int octave) {
		this->oct = octave;
	}

	// get y for note
	void getNoteY(const Note n, int& y, int& h) {
		int i = n.getNoteNr() - oct*12;
		getNoteY(i, y, h);
	}

	// get y for note-number
	void getNoteY(const int n, int& y, int& h) {
		y = this->y + noteH*11 - n*noteH + 1;
		h = noteH - 2;
	}

	// get x for beat
	void getBeatX(const int tick, int& x, int& w) {
		x = this->x + (tick * noteW);
		w = noteW;
	}

	/** convert (x,y) to note(number) and tick */
	void getNoteTick(const int x, const int y, int& noteNumber, int& tick) {
		tick = (x - this->x) / noteW;
		noteNumber = 11 - (y - this->y - 2) / noteH;
		//noteNumber = floor(this->y - y - 1);// / noteH;
		if (noteNumber < 0 || noteNumber >= 12) {noteNumber = -1;} else {noteNumber += oct*12;}
	}

	/** ctor */
	Sequencer1Scaler() {;}

};



/**
 * @brief The Sequencer1 class
 */
class Sequencer1 : public RackElement, public PatternSequencer {
	Q_OBJECT

public:

	/** ctor */
	explicit Sequencer1(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~Sequencer1();


	//std::string getDeviceType() const override;

	//RackElement* createFromPrototype(Context& ctx) const override;

	void refresh() override;

//	std::string getProductString() const;

protected:

	void paintEvent (QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;


private:

	friend class Sequencer1Selector;

	/** scale between gui and notes */
	struct Sequencer1Scaler mscaler;

	struct {

		ImgSpin* spnBars;
		ImgSpin* spnGranularity;
		ImgSpin* spnOctave;

		CheckBox* chkPattern[16];
		CheckBoxGroup* grpPattern;

	} elements;

	/** note selector */
	Sequencer1Selector* selector;


signals:

public slots:

	void onValueChanged();

	void onPatternSelect();

};

#endif // SEQUENCER1_H
