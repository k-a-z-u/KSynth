#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
class SystemSettings;
class SoundSink;

namespace Ui {
	class SettingsDialog;
}


/**
 * dialog to configure the most import system settings
 * like audio output, gui parameters, export configurations, etc.
 */
class SettingsDialog : public QDialog {
	Q_OBJECT

public:

	/** ctor */
	explicit SettingsDialog(SystemSettings& settings, QWidget* parent = 0);

	/** dtor */
	~SettingsDialog();

private:

	Ui::SettingsDialog *ui;

	/** the underlying model */
	SystemSettings& settings;


	void showLatency(SoundSink* ss);

private slots:

	void onCancel();

	void onOk();


	void onRefreshChanged(int val);

	void onSelectSoundSink(int idx);

};

#endif // SETTINGSDIALOG_H
