#ifndef TASKS_H
#define TASKS_H

#include <QObject>
#include <QSemaphore>
#include "Task.h"
#include <queue>
#include <thread>

class ProgressDialog;

/**
 * this class provides task handling facilities to execute
 * tasks in either foreground (main thread) or background (some other thread)
 * and provides a GUI window to indicate the current state.
 *
 */
class Tasks : public QObject {
	Q_OBJECT


public:

	/** ctor */
	Tasks(QWidget* parent = nullptr);

	/** dtor */
	~Tasks();


	/** add a new task to execute in foreground */
	void addTaskForeground(Task* t);

	/** add a new task to execute in background */
	void addTaskBackground(Task* t);


private:

	/** run background task */
	void runInBackground();

	/** execute the given task and show GUI status */
	void exec(Task* t, bool foreground);


private:

	/** all tasks to execute */
	std::queue<Task*> tasks;

	/** signalling */
	QSemaphore sema;

	/** the executing thread */
	std::thread* thread;

	/** visible status dialog */
	ProgressDialog* dialog;

	/** task subsystem active? */
	bool enabled;


private slots:

	/** task's progress changed */
	void onTaskProgress(TaskStatus s);

	/** task is starting */
	void onTaskStart();

	/** task is done */
	void onTaskDone();

	/** task encountered an error */
	void onTaskError(QString err);

signals:

	/** work done */
	void onDone(Task* t);

};

#endif // TASKS_H
