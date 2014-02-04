#include "Task.h"

Task::Task(const std::string& name, bool deleteOnDone) : name(name), deleteOnDone(deleteOnDone) {
	;
}

Task::~Task() {
	;
}

void Task::setProgress(const std::string& msg) {
	status.msg = msg;
	emit onProgress(status);
}

void Task::setProgress(float val) {
	status.val = val;
	emit onProgress(status);
}

void Task::setProgress(const std::string& msg, float val) {
	status.val = val;
	status.msg = msg;
	emit onProgress(status);
}

void Task::setError(const std::string& error) {
	emit onError( QString(error.c_str()) );
}

void Task::run() {
	emit onStart();
	this->exec();
	emit onDone();
}
