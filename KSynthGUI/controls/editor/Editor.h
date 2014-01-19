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


#include "EditorMode.h"
#include "EditorScaler.h"
#include "KSynth/SequencerListener.h"


/**
 * provides editing for notes, and probably many other things
 * in the future.
 *
 * editing is done per SequencerTrack.
 *
 */
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
	TimeBase128 getSongLength() const;


	/** convert midi events of the given track to notes (combined on+off) */
	std::vector<EditorNote> getNotes(SequencerTrack& st) const;

	/** get the widget to use for the left header */
	QWidget* getHeaderWidget() const;

	/**
	 * change the editor's current mode:
	 * cursor: editing of existing entries
	 * draw: add new entries
	 */
	void setMode(EditorMode mode);

	/** get the editor's current mode */
	EditorMode getMode() const;

	/** get the time-slider */
	EditorSlider* getSlider() const;

	/** get the editor's working context */
	Context& getContext() const;

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

	/** the slider to show/select the current playback positin */
	EditorSlider* slider;

	/** the editor's current mode */
	EditorMode mode;

signals:


public slots:

	void zoomUp();

	void zoomDown();


private slots:

	/** the selected track changed */
	void onTrackSelectionChanged(SequencerTrack* st);


};

#endif // EDITOR_H
