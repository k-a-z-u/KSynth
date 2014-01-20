#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
	class ProgressDialog;
}

/**
 * progress indicator for longer tasks like:
 * loading workspace, building LUTs, ...
 */
class ProgressDialog : public QDialog {
	Q_OBJECT

public:

	/** ctor */
	explicit ProgressDialog(QWidget *parent = 0);

	/** dtor */
	~ProgressDialog();


	/** set the text to display */
	void setText(const std::string& msg);

	/** set the current progress [0:1] */
	void setProgress(float percent);


private:
	Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
