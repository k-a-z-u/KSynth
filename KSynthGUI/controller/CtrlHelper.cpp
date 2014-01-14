#include "CtrlHelper.h"

#include <KLib/fs/File.h>


#include <QFileDialog>
std::string CtrlHelper::openFile(const std::string& caption, const std::string& filterDesc, const std::string& filter) {

	QFileDialog dialog;
	std::string fs = filterDesc + "(" + filter + ")";
	dialog.setNameFilter(fs.c_str());
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setWindowTitle(caption.c_str());
	if (dialog.exec()) {
		std::string file = dialog.selectedFiles().at(0).toStdString();
		return file;
	}
	return "";

}


std::string CtrlHelper::saveFile(const std::string& caption, const std::string& fileDesc, const std::string& fileExt) {

	std::string fs = fileDesc + "(*." + fileExt + ")";
	std::string file = QFileDialog::getSaveFileName(0, caption.c_str(), "", fs.c_str()).toStdString();
	if (file.empty()) {return "";}

	// append (missing) extension
	std::string curExt = file.substr( file.length() - fileExt.length(), fileExt.length() );
	if (curExt != fileExt) {file += "." + fileExt;}

	return file;

}
