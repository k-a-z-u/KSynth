#include "Task.h"

Task::Task(const std::string& name, bool deleteOnDone) : name(name), deleteOnDone(deleteOnDone) {
	;
}

Task::~Task() {
	;
}

/** set progress string */
void Task::setProgress(const std::string& msg) {
	status.msg = msg;
	emit onProgress(status);
}

/** set progress value */
void Task::setProgress(float val) {
	status.val = val;
	emit onProgress(status);
}

/** set progress string and value */
void Task::setProgress(const std::string& msg, float val) {
	status.val = val;
	status.msg = msg;
	emit onProgress(status);
}
