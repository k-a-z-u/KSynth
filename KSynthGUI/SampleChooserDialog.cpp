#include "SampleChooserDialog.h"
#include "ui_SampleChooserDialog.h"

#include <QFileSystemModel>
#include <QModelIndex>


SampleChooserDialog::SampleChooserDialog(const K::File& startFolder, QWidget *parent) :
	QDialog(parent), ui(new Ui::SampleChooserDialog) {

	ui->setupUi(this);

	// create directory model
	K::File root("samples");
	std::string rootPath =  root.getAbsolutePath();
	dirModel = new QFileSystemModel(this);
	dirModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
	ui->treeFolders->setModel(dirModel);
	ui->treeFolders->setRootIndex(dirModel->setRootPath( rootPath.c_str() ));
	ui->treeFolders->hideColumn(1);
	ui->treeFolders->hideColumn(2);
	ui->treeFolders->hideColumn(3);

	// create file model
	fileModel = new QFileSystemModel(this);
	fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	ui->listFiles->setModel(fileModel);
	//ui->listFiles->setRootIndex(fileModel->setRootPath( rootPath.c_str() ));

	// select start path
	std::string startPath = startFolder.getAbsolutePath();
	ui->treeFolders->setCurrentIndex(dirModel->index( startPath.c_str() ));
	ui->listFiles->setRootIndex(fileModel->setRootPath( startPath.c_str() ));

	// signals
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));

	connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(onOK()));

	connect(ui->treeFolders->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
			this, SLOT(onFolderSelect(QModelIndex, QModelIndex)));

	connect(ui->listFiles->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
			this, SLOT(onFileSelect(QModelIndex, QModelIndex)));

}

SampleChooserDialog::~SampleChooserDialog() {
	delete ui;
}

std::string SampleChooserDialog::openSampleFile(const std::string& caption, QWidget* parent) {
	return openSampleFile(caption, K::File("samples"), parent);
}

std::string SampleChooserDialog::openSampleFile(const std::string& caption, const K::File& folder, QWidget* parent) {

	SampleChooserDialog dia(folder, parent);
	dia.setWindowTitle(caption.c_str());
	dia.setModal(true);
	dia.exec();

	return (dia.current.valid) ? (dia.current.file.getAbsolutePath()) : ("");

}


void SampleChooserDialog::onCancel() {
	current.valid = false;
	close();
}

void SampleChooserDialog::onOK() {
	close();
}

void SampleChooserDialog::onFolderSelect(const QModelIndex &index, const QModelIndex& old) {
	(void) old;
	QString mPath = dirModel->fileInfo(index).absoluteFilePath();
	ui->listFiles->setRootIndex(fileModel->setRootPath(mPath));
}

void SampleChooserDialog::onFileSelect(const QModelIndex &index, const QModelIndex& old) {
	(void) old;
	QString mFile = fileModel->fileInfo(index).absoluteFilePath();
	K::File file(mFile.toUtf8().constData());

	ui->lblSize->setText( (std::to_string(file.length() / 1024) + " KiB").c_str() );

	// try to load sample
	try {
		current.sample = SampleFactory::load(file);
		ui->lblSampleRate->setText( std::to_string(current.sample.getSampleRate()).c_str() );
		ui->lblChannels->setText( std::to_string(current.sample.getNumChannels()).c_str() );
		ui->lblRawSize->setText( (std::to_string(current.sample.getMemoryUsage() / 1024) + " KiB").c_str()) ;
		ui->lblLength->setText( (std::to_string(current.sample.getLengthMilliSeconds()) + " ms").c_str());
		ui->lblError->setText("-");
		current.file = file;
		current.valid = true;

	} catch (std::exception& e) {
		ui->lblSampleRate->setText("-");
		ui->lblChannels->setText("-");
		ui->lblRawSize->setText("-");
		ui->lblError->setText(e.what());
		ui->lblLength->setText("-");
		current.valid = false;

	}

}

