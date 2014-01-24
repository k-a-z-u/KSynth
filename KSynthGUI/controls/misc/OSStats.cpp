#include "OSStats.h"


#include <QProgressBar>
#include <QHBoxLayout>
#include <QLabel>

OSStats::OSStats(QWidget* parent) : QWidget(parent) {

	thread = new std::thread(&OSStats::run, this);

	QHBoxLayout* lay = new QHBoxLayout();

	barCpu = new QProgressBar(); barCpu->setObjectName("barCPU"); barCpu->setFormat("CPU: %p%");
	barMem = new QProgressBar(); barMem->setObjectName("barMEM"); barMem->setFormat("MEM: %v MB");

	barCpu->setAlignment(Qt::AlignCenter);
	barMem->setAlignment(Qt::AlignCenter);

	lay->addWidget(barCpu);
	lay->addWidget(barMem);

	lay->setMargin(0);
	lay->setSpacing(1);
	this->setLayout(lay);

}

OSStats::~OSStats() {
	thread->detach();
	delete thread;
}

void OSStats::update() {

	int cpuUsage = int(  cpu.getUsageSinceLastCall() * 100 );
	barCpu->setValue( cpuUsage );

	barMem->setMaximum( int(mem.getTotalMemory() / 1024 / 1024) );
	barMem->setValue( int(mem.getProcessUsedMemory() / 1024 / 1024) );

}

void OSStats::run() {


	while(true) {

		QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}

}
