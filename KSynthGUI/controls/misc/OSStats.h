#ifndef OSSTATS_H
#define OSSTATS_H

#include <QWidget>
#include <thread>
class QLabel;
class QProgressBar;
#include <KLib/os/CPUUsage.h>
#include <KLib/os/MemoryUsage.h>

/**
 * show some OS stats (cpu/memory) within the
 * main window's status bar
 */
class OSStats : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit OSStats(QWidget* parent = 0);

	/** dtor */
	~OSStats();

private slots:

	void update();

private:

	/** refresh */
	void run();

	/** refresh thread */
	std::thread* thread;

	QProgressBar* barCpu;
	QProgressBar* barMem;

	K::CPUUsage cpu;
	K::MemoryUsage mem;

};

#endif // OSSTATS_H
