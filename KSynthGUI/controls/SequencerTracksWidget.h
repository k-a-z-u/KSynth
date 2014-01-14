#ifndef SEQUENCERTRACKSWIDGET_H
#define SEQUENCERTRACKSWIDGET_H

#include <QScrollArea>
#include <KSynth/SequencerListener.h>
class Context;
class QVBoxLayout;

class SequencerTracksWidget : public QWidget, public SequencerTrackListener {
	Q_OBJECT

public:

	/** ctor */
	explicit SequencerTracksWidget(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~SequencerTracksWidget();

protected:

	void onTracksChanged();

private:

	Context& ctx;

	QVBoxLayout* lay;

private slots:

	void refreshMe();

signals:

public slots:

};

#endif // SEQUENCERTRACKSWIDGET_H
