#include "CtrlHelper.h"

#include <KLib/fs/File.h>

#include <QFileDialog>

std::string CtrlHelper::getFilterString(std::vector<FileDialogFilter> filter) {
	std::string ret = "";
	for (const FileDialogFilter& f : filter) {
		ret += f.desc + " (*." + f.ext + ");;";
	}
	return ret.substr(0, ret.length()-2);
}



std::string CtrlHelper::openFile(const std::string& caption, FileDialogFilter filter) {
	std::vector<FileDialogFilter> vec;
	vec.push_back(filter);
	return openFile(caption, vec);
}

std::string CtrlHelper::openFile(const std::string& caption, std::vector<FileDialogFilter> filter) {

	QFileDialog dialog;
	std::string fs = getFilterString(filter);
	dialog.setNameFilter(fs.c_str());
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setWindowTitle(caption.c_str());
	if (dialog.exec()) {
		std::string file = dialog.selectedFiles().at(0).toUtf8().constData();
		return file;
	}
	return "";

}

std::string CtrlHelper::saveFile(const std::string& caption, FileDialogFilter filter) {
	std::vector<FileDialogFilter> vec;
	vec.push_back(filter);
	return saveFile(caption, vec);
}

std::string CtrlHelper::saveFile(const std::string& caption, std::vector<FileDialogFilter> filter) {

	// show chooser
	QString selFilter;
	std::string fs = getFilterString(filter);
	std::string file = QFileDialog::getSaveFileName(0, caption.c_str(), "", fs.c_str(), &selFilter).toUtf8().constData();
	if (file.empty()) {return "";}

	// get selected file-extension from filter
	int p1 = selFilter.lastIndexOf("(*.");
	int p2 = selFilter.lastIndexOf(")");
	std::string fileExt = selFilter.mid(p1+3, p2-p1-3).toUtf8().constData();

	// append (missing) extension
	std::string curExt = file.substr( file.length() - fileExt.length(), fileExt.length() );
	if (curExt != fileExt) {file += "." + fileExt;}

	return file;

}
