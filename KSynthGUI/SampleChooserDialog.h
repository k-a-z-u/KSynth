#ifndef SAMPLECHOOSERDIALOG_H
#define SAMPLECHOOSERDIALOG_H

#include <QDialog>

class QFileSystemModel;
class QModelIndex;

#include <KLib/fs/File.h>
#include <KSynth/sampler/SampleFactory.h>

namespace Ui {
	class SampleChooserDialog;
}

/**
 * choose audio-sample to load from fixed folder and sub-folders
 * with live playback.
 */
class SampleChooserDialog : public QDialog {
	Q_OBJECT

public:

	/** dtor */
	~SampleChooserDialog();

	/** show a dialog to choose a sample file */
	static std::string openSampleFile(const std::string& caption, QWidget* parent = 0);

	/** show a dialog to choose a sample file, start showing the given folder */
	static std::string openSampleFile(const std::string& caption, const K::File& folder, QWidget* parent = 0);

private:

	/** hidden ctor. use static function! */
	explicit SampleChooserDialog(const K::File& startFolder, QWidget *parent = 0);


	Ui::SampleChooserDialog *ui;

	/** model to list directories */
	QFileSystemModel *dirModel;

	/** model to list files */
	QFileSystemModel *fileModel;


	struct Current {

		/** the currently loaded sample */
		Sample sample;

		/** the currently loaded file */
		K::File file;

		/** current selection is valid? */
		bool valid;

		/** ctor */
		Current() : valid(false) {;}

	} current;


private slots:

	void onCancel();

	void onOK();

	void onFolderSelect(const QModelIndex& index, const QModelIndex& old);

	void onFileSelect(const QModelIndex& index, const QModelIndex& old);

};

#endif // SAMPLECHOOSERDIALOG_H
