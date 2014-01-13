#ifndef CTRL_HELPER_H
#define CTRL_HELPER_H

#include <string>

namespace K {
	class File;
}

class CtrlHelper {

public:

	static std::string openFile(const std::string& caption, const std::string& filterDesc, const std::string& filter);

	static std::string saveFile(const std::string& caption, const std::string& fileDesc, const std::string& fileExt);

};

#endif // CTRL_HELPER_H
