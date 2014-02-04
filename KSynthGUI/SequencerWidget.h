#ifndef SEQUENCERWIDGET_H
#define SEQUENCERWIDGET_H

#include <QDockWidget>

class Context;
class Editor;
class SequencerTracksWidget;

namespace Ui {
	class SequencerWidget;
}

class SequencerWidget : public QDockWidget {
	Q_OBJECT


public:

	/** ctor */
	explicit SequencerWidget(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~SequencerWidget();


private:

	Ui::SequencerWidget *ui;

	Context& ctx;

	Editor* editor;

	/** the track selector */
	SequencerTracksWidget* tracks;



private slots:

	/** change the editors mode */
	void changeMode();

	/** change snapping mode */
	void changeSnap();

	/** change snapping interval */
	void changeSnapInterval();

	/** add a new track */
	void addTrack();

	/** delete the currently selected track */
	void deleteTrack();

};

#endif // SEQUENCERWIDGET_H
