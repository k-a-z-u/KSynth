#ifndef CONTROLPANELWIDGET_H
#define CONTROLPANELWIDGET_H

#include <KSynth/SequencerListener.h>

#include <QDockWidget>
class Context;

namespace Ui {
	class ControlPanelWidget;
}

class ControlPanelWidget : public QDockWidget, public SequencerStatusListener, public SequencerBeatListener {
	Q_OBJECT


public:

	/** ctor */
	explicit ControlPanelWidget(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~ControlPanelWidget();


protected:

	void onBeat(Beat beat, Time time) override;

	void onStatusChange(SequencerStatus status) override;

	void onSettingsChange() override;

private slots:

	void onStop();

	void onPlay();

	void onRec();

	void onBPM();

	void _onBeat(unsigned int beat, float time);

private:

	Ui::ControlPanelWidget *ui;

	Context& ctx;

};

#endif // CONTROLPANELWIDGET_H
