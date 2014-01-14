#ifndef CTRL_HELPER_H
#define CTRL_HELPER_H

#include <string>
#include <vector>

namespace K {
	class File;
}

struct FileDialogFilter {
	std::string desc;
	std::string ext;
	FileDialogFilter(const std::string& desc, const std::string& ext) :
		desc(desc), ext(ext) {;}
};

class CtrlHelper {

public:

	static std::string openFile(const std::string& caption, FileDialogFilter filter);

	static std::string openFile(const std::string& caption, std::vector<FileDialogFilter> filter);

	static std::string saveFile(const std::string& caption, FileDialogFilter filter);

	static std::string saveFile(const std::string& caption, std::vector<FileDialogFilter> filter);

private:

	/** create filter string from given vector */
	static std::string getFilterString(std::vector<FileDialogFilter> filter);

};

#endif // CTRL_HELPER_H
