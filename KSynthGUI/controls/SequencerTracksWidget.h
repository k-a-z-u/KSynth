#ifndef SEQUENCERTRACKSWIDGET_H
#define SEQUENCERTRACKSWIDGET_H

#include <QScrollArea>
#include <KSynth/SequencerListener.h>
class Context;
class QVBoxLayout;
class SequencerTrackItem;
class SequencerTrack;

/** listen for track selection */
class SequencerTracksListener {

public:

	/** dtor */
	virtual ~SequencerTracksListener();

	/** the selected track has changed */
	virtual void selectedTrackChanged(SequencerTrack* st);

};



class SequencerTracksWidget : public QWidget, public SequencerTrackListener {
	Q_OBJECT

public:

	/** ctor */
	explicit SequencerTracksWidget(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~SequencerTracksWidget();

	/** the currently selected item */
	SequencerTrackItem* getSelectedItem();


//	/** add a new listener */
//	void addListener(SequencerTracksListener* l);

//	/** remove an existing listener */
//	void removeListener(SequencerTracksListener* l);


protected:

	void onTracksChanged();

	friend class SequencerTrackItem;
	void setSeleceted(SequencerTrackItem& item);


private:

	Context& ctx;

	QVBoxLayout* lay;

	/** the currently selected item (if any) */
	SequencerTrackItem* selected;

	/** all listeners */
	std::vector<SequencerTracksListener*> listeners;

private slots:

	void refreshMe();


signals:

	/** the selected item has changed */
	void onCurrentItemChanged(SequencerTrack* st);


};

#endif // SEQUENCERTRACKSWIDGET_H
