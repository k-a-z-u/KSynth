#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
class SystemSettings;
class SoundSinkHardware;

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

	/** dtor */
	~SettingsDialog();

	/** show settings dialog. returns true on "ok" and false on "cancel" */
	static bool show(SystemSettings& settings, QWidget* parent = 0);

private:

	/** hidden ctor. use singleton */
	explicit SettingsDialog(SystemSettings& settings, QWidget* parent = 0);

private:



	Ui::SettingsDialog *ui;

	/** the underlying model */
	SystemSettings& settings;

	bool okClicked;

	void showLatency(SoundSinkHardware* ss);

private slots:

	void onCancel();

	void onOk();


	void onRefreshChanged(int val);

	void onSelectSoundSink(int idx);

};

#endif // SETTINGSDIALOG_H
