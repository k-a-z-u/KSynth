#ifndef EDITORSCALER_H
#define EDITORSCALER_H


#define ES_MAX_NOTE	108
#define ES_MIN_NOTE	12

/**
 * helper class to translate between pixels and beats etc..
 * needed to draw elements by timing information and convert
 * pixels back to timings
 */
class EditorScaler {

public:

	/** ctor */
	EditorScaler() : sx(0), sy(0), base(4) {;}

	unsigned int getSX() const {return sx;}
	unsigned int getSY() const {return sy;}

	unsigned int getNW() const {return nw4 * base / 4;}
	unsigned int getNH() const {return nh4;}

	unsigned int getBarWidth() const {return nw4 * base;}

	unsigned int getNumBars(unsigned int len) const {return len * 4 / 128;}

	unsigned int getBase() const {return base;}
	void setBase(unsigned int base) {
		if (base >= 1) {this->base = base;}
	}

	/** convert multiples of 128th notes to pixels */
	unsigned int getObjectWidth(unsigned int len128) const {return sx + nw4 * len128 * base / 128;}

	/** convert note's length (multiples of 128th notes) to pixels */
	unsigned int getNoteWidth(EditorNote& en) const {return (nw4 * en.getLength() * base) / 128;}

	/** convert x-coordinate (in pixels) back to multiples of 128th notes */
	unsigned int getBeat128(unsigned int x) {return x * 128 / nw4 / base;}


	/** convert midi note number to pixels */
	unsigned int getNoteY(unsigned int noteNr) {return (ES_MAX_NOTE - noteNr) * nh4;}

	/** convert y pixels back to midi note number */
	unsigned int getNoteNr(unsigned int y) {return ES_MAX_NOTE - (y / nh4);}

	/** convert x pixels to note delay (multiples of 128th notes) */
	unsigned int getNoteDelay(unsigned int x) {return getBeat128(x);}


private:

	/** move left */
	unsigned int sx;

	/** move down */
	unsigned int sy;

	/** zooming factor. default: display 4th notes */
	unsigned int base;

	/** note width at base = 1 */
	const int nw4 = 16;

	/** note height */
	const int nh4 = 8;

};


#endif // EDITORSCALER_H
