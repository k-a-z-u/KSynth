#include "Tasks.h"
#include "../../ProgressDialog.h"

#include <QApplication>
#include <QMetaType>

Tasks::Tasks(QWidget* parent) : enabled(true) {

	// start background thread
	thread = new std::thread(&Tasks::runInBackground, this);

	// visible status
	dialog = new ProgressDialog(parent);

	qRegisterMetaType<TaskStatus>("TaskStatus");

}

Tasks::~Tasks() {

	if (thread) {

		// switch to disabled
		enabled = false;

		// release the semaphore.
		// due to "enabled=false": loop terminates
		sema.release();

		// cleanup
		thread->join();
		delete thread;
		thread = nullptr;

	}

	delete dialog;
	dialog = nullptr;

}

void Tasks::addTaskForeground(Task* t) {

	// execute
	exec(t, true);

}


void Tasks::addTaskBackground(Task* t) {

	// enqueue
	tasks.push(t);
	sema.release();

}


void Tasks::runInBackground() {

	while (enabled) {

		// wait for next task
		sema.acquire();
		if (!enabled) {break;}

		// get next task
		Task* t = tasks.front();
		tasks.pop();

		// execute
		exec(t, false);

		int i = 0;

	}

}


void Tasks::exec(Task* t, bool foreground) {

	// setup signals
	connect(t, SIGNAL(onStart()), this, SLOT(onTaskStart()), Qt::AutoConnection);
	connect(t, SIGNAL(onProgress(TaskStatus)), this, SLOT(onTaskProgress(TaskStatus)), Qt::AutoConnection);
	connect(t, SIGNAL(onDone()), this, SLOT(onTaskDone()), Qt::AutoConnection);
	connect(t, SIGNAL(onError(QString)), this, SLOT(onTaskError(QString)), Qt::AutoConnection);

	// run (will trigger start, progress, done)
	t->run();

	// done
	//TaskStatus ts("", 1);
	//emit onTaskProgress(ts);

	// the task will be deleted, after he sent the onDone() signal
	// AND this signal has been processed by the event loop.
	// see "Tasks::onTaskDone()" below.

}

void Tasks::onTaskProgress(TaskStatus s) {

	dialog->setText(s.msg);
	dialog->setProgress(s.val);
	QApplication::processEvents();
	//std::this_thread::sleep_for( std::chrono::milliseconds(5) );

}

void Tasks::onTaskDone() {

	// hide progress dialog
	std::this_thread::sleep_for( std::chrono::milliseconds(150) );
	dialog->setVisible(false);

	// cleanup?
	Task* t = (Task*) sender();
	if (t->deleteAfterCompletion()) {delete t;}

}

#include <QMessageBox>
void Tasks::onTaskError(QString str) {
	QMessageBox::information(0, "task error", str);
}

void Tasks::onTaskStart() {

	Task* t = (Task*) sender();

	// show progress dialog
	dialog->setWindowTitle( t->getName().c_str() );
	dialog->setModal(true);
	dialog->setVisible(true);
	dialog->setProgress(0);
	dialog->setText("");
	QApplication::processEvents();
	std::this_thread::sleep_for( std::chrono::milliseconds(150) );

}
