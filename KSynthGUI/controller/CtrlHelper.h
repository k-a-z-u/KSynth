#ifndef CTRL_HELPER_H
#define CTRL_HELPER_H

#include <string>
#include <vector>

namespace K {
	class File;
}

/**
 * describes one file-format entry for a file-open / file-save
 * dialog using a description for the format and its extension
 */
struct FileDialogFilter {

	/** a description of the format */
	std::string desc;

	/** the formats extension (without the ".") */
	std::string ext;

	/** a user-data pointer */
	void* ptr;

	/** emoty ctor */
	FileDialogFilter() : desc(), ext(), ptr(nullptr) {;}

	/**
	 * ctor
	 * @param desc a description of the format
	 * @param ext the formats extension (without the ".")
	 * @param ptr pointer to user-data. useful when the dialog returns user's filter-selection
	 */
	FileDialogFilter(const std::string& desc, const std::string& ext, void* ptr = nullptr) :
		desc(desc), ext(ext), ptr(ptr) {;}

	/** get filter string for this filter. format: "desc (*.ext)" */
	std::string getFilterString() const {
		return desc + " (*." + ext + ")";
	}

};



class CtrlHelper {

public:

	/**
	 * show a new file-open dialog
	 * @param caption the window title
	 * @param filter the filter (file extension) to apply
	 * @return the selected file, or an empty string
	 */
	static std::string openFile(const std::string& caption, FileDialogFilter filter);

	/**
	 * show a new file-open dialog
	 * @param caption the window title
	 * @param filter list of allowed file extensions
	 * @param selection the selected extension after closing the dialog
	 * @return the selected file, or an empty string
	 */
	static std::string openFile(const std::string& caption, std::vector<FileDialogFilter> filter, FileDialogFilter* selection = 0);

	/**
	 * show a new file-save dialog
	 * @param caption the window title
	 * @param filter the filter (file extension) to apply
	 * @return the selected file, or an empty string
	 */
	static std::string saveFile(const std::string& caption, FileDialogFilter filter);

	/**
	 * show a new file-save dialog
	 * @param caption the window title
	 * @param filter list of allowed file extensions
	 * @param selection the selected extension after closing the dialog
	 * @return  the selected file, or an empty string
	 */
	static std::string saveFile(const std::string& caption, std::vector<FileDialogFilter> filter, FileDialogFilter* selection = 0);

private:

	/** create filter string from given vector */
	static std::string getFilterString(std::vector<FileDialogFilter> filter);

};

#endif // CTRL_HELPER_H
