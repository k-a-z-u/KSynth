#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "EditorNote.h"

class EditorSheet;
class EditorSheetHeader;
class EditorSlider;
class Context;
class SequencerTracksWidget;
class SequencerTrack;
class QScrollArea;
class QVBoxLayout;
class QScrollBar;

#include "KSynth/SequencerListener.h"

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

	unsigned int getObjectWidth(unsigned int len) const {return sx + nw4 * len * base / 128;}

	unsigned int getNoteWidth(EditorNote& en) const {return (nw4 * en.getLength() * base) / 128;}

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




class Editor : public QWidget, public SequencerBeatListener {
	Q_OBJECT


public:

	/** ctor */
	explicit Editor(Context& ctx, SequencerTracksWidget& tracks, QWidget *parent = 0);

	/** dtor */
	~Editor();


	/** get scaling element */
	EditorScaler& getScaler();

	/** get the song's length in multiples of 128th notes */
	unsigned int getSongLength();


	/** convert midi events of the given track to notes (combined on+off) */
	std::vector<EditorNote> getNotes(SequencerTrack& st);

	/** get the widget to use for the left header */
	QWidget* getHeaderWidget() const;

protected:

	void onBeat(Beat beat, Time time) override;



private:

	void resizeMe();

	Context& ctx;

	/** the track-selection widget */
	SequencerTracksWidget& tracks;

	/** scaling of all entries */
	EditorScaler scaler;

	/** dummy widget to hold the left header */
	QWidget* header;

	QVBoxLayout* layout;
	QVBoxLayout* layoutHeader;

	EditorSheet* sheet;
	EditorSheetHeader* sheetHeader;

	EditorSlider* slider;


signals:


public slots:

	void zoomUp();

	void zoomDown();


private slots:

	/** the selected track changed */
	void onTrackSelectionChanged(SequencerTrack* st);


};

#endif // EDITOR_H
