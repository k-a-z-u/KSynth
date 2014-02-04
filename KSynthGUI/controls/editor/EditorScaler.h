#ifndef EDITORSCALER_H
#define EDITORSCALER_H


#define ES_MAX_NOTE	108
#define ES_MIN_NOTE	12

#include "EditorSheetNoteModel.h"
#include <QRect>


/**
 * helper class to translate between pixels and beats etc..
 * needed to draw elements by timing information and convert
 * pixels back to timings.
 *
 * some kind of glue between backend models and the GUI.
 *
 */
class EditorScaler {

public:

	/** ctor */
	EditorScaler() : sx(0), sy(0), base(4) {;}

	/** start offset for x coordinate */
	unsigned int getSX() const {return sx;}

	/** start offset for y coordinate */
	unsigned int getSY() const {return sy;}


	/** get the height of one note */
	unsigned int getNH() const {return nh4;}
	unsigned int getNW() const {return nw4 * base / 4;}


	unsigned int getBarWidth() const {return nw4 * base;}

	/** get the number of bars (4/4 beat) for the given song-length (in 128th notes) */
	unsigned int getNumBars(unsigned int len) const {return len * 4 / 128;}


	/** set the width of one 4th note. (used for zooming) */
	void set4thWidth(unsigned int pixel) {
		if (pixel >= 4) {nw4 = pixel;}
	}

	/** get the width of one 4th note. (used for zooming) */
	unsigned int get4thWidth() {
		return nw4;
	}

	/** convert multiples of 128th notes to pixels */
	unsigned int getObjectWidth(unsigned int len128) const {return sx + nw4 * len128 * base / 128;}

	/** convert note's length (multiples of 128th notes) to pixels */
	unsigned int getNoteWidth(EditorSheetNoteModel& en) const {return (nw4 * en.getLength() * base) / 128;}

	/** convert x-coordinate (in pixels) back to multiples of 128th notes */
	TimeBase128 getTime128(unsigned int x) {return x * 128 / nw4 / base;}


	/** convert midi note number to pixels */
	unsigned int getNoteY(unsigned int noteNr) {return (ES_MAX_NOTE - noteNr) * nh4;}

	/** convert y pixels back to midi note number */
	unsigned int getNoteNr(unsigned int y) {return ES_MAX_NOTE - (y / nh4);}

	/** convert x pixels to note delay (multiples of 128th notes) */
	TimeBase128 getNoteDelay(unsigned int x) {return getTime128(x);}



	/** set note's number/timing from the given coordinates */
	void setNote(EditorSheetNoteModel& mdl, int x, int y, int w) {
		unsigned int evtOn = getNoteDelay(x);
		unsigned int evtOff = getNoteDelay(x+w);
		unsigned int noteNr = getNoteNr(y);

		mdl.setNoteNumber(noteNr);
		if (snap.enabled) {
			mdl.setTimes(evtOn, evtOff, snap.base);
		} else {
			mdl.setTimes(evtOn, evtOff);
		}

	}

	/** get note's geometry from the given number/timings */
	QRect getNote(EditorSheetNoteModel& mdl) {
		int x = getObjectWidth(mdl.getOnEvt()->delay);
		int y = getNoteY(mdl.getNoteNumber());
		int w = getNoteWidth(mdl);
		int h = nh4;
		if (w < 4) {w = 4;}
		return QRect(x,y,w,h);
	}


	/** snapping configuration */
	struct Snap {

		/** is snapping enabled? */
		bool enabled;

		/** the base to snap to (e.g. 4th notes) */
		int base;

		/** ctor */
		Snap() : enabled(false), base(4) {;}

		/** get the changing interval (negated snapping setting) */
		TimeBase128 getSteps() {return 128 / base;}

	} snap;


private:

	/** left offset for everything within the editor */
	unsigned int sx;

	/** top offset for everything within the editor */
	unsigned int sy;

	/** zooming factor. default: display 4th notes */
	unsigned int base;

	/** note width for 4th notes */
	int nw4 = 16;

	/** note height */
	const int nh4 = 8;

};


#endif // EDITORSCALER_H
