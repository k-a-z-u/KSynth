#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <string>

struct TaskStatus {
	std::string msg;
	float val;
	TaskStatus(const std::string& msg, float val) : msg(msg), val(val) {;}
	TaskStatus() : msg(""), val(0) {;}
};



/**
 * interface for a task.
 *
 * tasks can be either executed in foreground (main thread)
 * or in the background (some other thread)
 *
 * tasks signal their progress which is then propagated
 * to some GUI element showing the task's current state
 */
class Task : public QObject {
	Q_OBJECT

public:

	/**
	 * ctor
	 * @param the task's name (GUI)
	 * @param deleteOnDone call delete for the task after it executed?
	 */
	Task(const std::string& name, bool deleteOnDone);

	/** dtor */
	virtual ~Task();

	/** run the task */
	void run();

	/** delete the task after completion? */
	bool deleteAfterCompletion() const {return deleteOnDone;}

	/** the task's name */
	const std::string getName() const {return name;}


protected slots:

	/** set progress string */
	void setProgress(const std::string& msg);

	/** set progress value (between [0,1]) */
	void setProgress(float val);

	/** set both, progress string and value */
	void setProgress(const std::string& msg, float val);

	/** an error occured */
	void setError(const std::string& error);

private:

	/** the work is perforemd within this method */
	virtual void exec() = 0;

	/** the worker's current status */
	TaskStatus status;

	/** the tasks name */
	std::string name;

	/** delete the task after it executed? */
	bool deleteOnDone;


signals:

	/** tasks progress has changed */
	void onProgress(TaskStatus status);

	/** the task is starting */
	void onStart();

	/** the task is done */
	void onDone();

	/** an error occured */
	void onError(QString str);

};

#endif // TASK_H
