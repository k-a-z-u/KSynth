#include "Tasks.h"
#include "../../ProgressDialog.h"
#include <QApplication>

Tasks::Tasks(QWidget* parent) {

	// start background thread
	thread = new std::thread(&Tasks::runInBackground, this);

	// visible status
	dialog = new ProgressDialog(parent);

}

Tasks::~Tasks() {

	if (thread) {
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

	while (true) {

		// wait for next task
		sema.acquire();

		// get next task
		Task* t = tasks.front();
		tasks.pop();

		// execute
		exec(t, false);

	}

}

void Tasks::exec(Task* t, bool foreground) {

	// setup signals
	connect(t, SIGNAL(onProgress(TaskStatus&)), this, SLOT(onProgress(TaskStatus&)));

	// show progress dialog
	dialog->setWindowTitle( t->name.c_str() );
	dialog->setModal(foreground);
	dialog->setVisible(true);
	dialog->setProgress(0);
	dialog->setText("");

	if (foreground) {QApplication::processEvents();}
	std::this_thread::sleep_for( std::chrono::milliseconds(250) );

	// run
	t->exec();

	// done
	TaskStatus ts("", 1);
	emit onProgress(ts);
	emit onDone(t);

	// done -> cleanup?
	if (t->deleteOnDone) {delete t;}

	// hide progress dialog
	std::this_thread::sleep_for( std::chrono::milliseconds(250) );
	dialog->setVisible(false);

}

void Tasks::onProgress(TaskStatus& s) {
	dialog->setText(s.msg);
	dialog->setProgress(s.val);
	QApplication::processEvents();
	std::this_thread::sleep_for( std::chrono::milliseconds(25) );
}
