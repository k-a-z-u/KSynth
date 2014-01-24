#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include "model/SystemSettings.h"
#include "model/SoundSinks.h"

class SoundSink;

SettingsDialog::SettingsDialog(SystemSettings& settings, QWidget *parent) :
	QDialog(parent), ui(new Ui::SettingsDialog), settings(settings) {

	ui->setupUi(this);

	// show current values
	ui->spnGuiRefreshInterval->setValue(settings.getGuiRefreshInterval());

	// available hardware sinks (and currently selected one)
	for (SoundSink* ss : SoundSinks::get().getHardwareSinks()) {
		ui->cmbSoundSink->addItem(ss->getName().c_str());
		if (settings.getSoundSink()->getName() == ss->getName()) {
			ui->cmbSoundSink->setCurrentIndex(ui->cmbSoundSink->count() - 1);
		}
	}
	showLatency(settings.getSoundSink());


	// connect signals
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
	connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(onOk()));

	connect(ui->spnGuiRefreshInterval, SIGNAL(valueChanged(int)), this, SLOT(onRefreshChanged(int)));

	connect(ui->cmbSoundSink, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectSoundSink(int)));

}

SettingsDialog::~SettingsDialog() {
	delete ui;
}

void SettingsDialog::showLatency(SoundSink* ss) {
	int latency = ss->getLatencyMS();
	std::string str = (latency == -1) ? ("unknown") : (std::to_string(latency) + " ms");
	ui->lblSoundSinkLatency->setText( str.c_str() );
}

void SettingsDialog::onCancel() {
	emit close();
}

void SettingsDialog::onOk() {
	emit close();
}

void SettingsDialog::onRefreshChanged(int val) {
	settings.setGuiRefreshInterval(val);
}

void SettingsDialog::onSelectSoundSink(int idx) {
	SoundSink* ss = SoundSinks::get().getHardwareSinks().at(idx);
	settings.setSoundSink(ss);
	showLatency(ss);
}
