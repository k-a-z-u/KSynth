#include "ControlPanelWidget.h"
#include "ui_ControlPanelWidget.h"
#include "model/Context.h"

#include <iomanip>
#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>



ControlPanelWidget::ControlPanelWidget(Context& ctx, QWidget *parent) :
	QDockWidget(parent), ui(new Ui::ControlPanelWidget), ctx(ctx) {

	ui->setupUi(this);

	connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(onStop()));
	connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(onPlay()));
	connect(ui->btnRec, SIGNAL(clicked()), this, SLOT(onRec()));
	connect(ui->spnBPM, SIGNAL(valueChanged(int)), this, SLOT(onBPM()));

	ctx.getSequencer()->bind(this);

}

static std::string timeToStr(Time t) {
	unsigned int msec = int( (t - Time(int(t))) * 1000 );
	unsigned int sec = (int)t % 60;
	unsigned int min = (int)t / 60 % 60;
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << min << ':';
	ss << std::setw(2) << std::setfill('0') << sec << ':';
	ss << std::setw(3) << std::setfill('0') << msec;
	return ss.str();
}

ControlPanelWidget::~ControlPanelWidget() {
	delete ui;
}




void ControlPanelWidget::onBeat(Beat beat, Time time) {
	QMetaObject::invokeMethod(this, "_onBeat", Qt::QueuedConnection, Q_ARG(unsigned int, beat), Q_ARG(float, time));
	//emit _onBeat(beat, time);
}

void ControlPanelWidget::onStatusChange(SequencerStatus status) {

	ui->btnPlay->setEnabled(status == SequencerStatus::STOPPED);
	ui->btnStop->setEnabled(status != SequencerStatus::STOPPED);
	ui->btnRec->setEnabled(status == SequencerStatus::STOPPED);

}

void ControlPanelWidget::_onBeat(unsigned int beat, float time) {

	// update only every few msec
	static int cnt = 0;
	if (++cnt < 25) {return;}
	cnt = 0;

	// display
	std::string t = timeToStr(time);
	ui->lblTime->setText( t.c_str() );
	ui->lblTick->setText( std::to_string(beat).c_str() );

}

void ControlPanelWidget::onStop() {
	ctx.getGenerator()->stop();
}

void ControlPanelWidget::onPlay() {
	ctx.getGenerator()->start();
}

void ControlPanelWidget::onRec() {
	ctx.getGenerator()->start();
}

void ControlPanelWidget::onBPM() {
	ctx.getSequencer()->setBeatsPerMinute(ui->spnBPM->value());
}
