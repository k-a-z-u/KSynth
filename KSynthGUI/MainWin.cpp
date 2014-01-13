#include "MainWin.h"
#include "ui_MainWin.h"

#include "model/Context.h"
#include "rack/RackFactory.h"
#include "controller/Controller.h"

MainWin::MainWin(Context& ctx, QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWin), ctx(ctx) {

	ui->setupUi(this);

	// available rack elements
	for (std::string& str : ctx.getRackFactory()->getDevices()) {
		QAction* act = ui->menuRackAddElement->addAction(str.c_str());
		act->setProperty("RackElement", QVariant(str.c_str()));
		connect(act, SIGNAL(triggered()), this, SLOT(addRackElement()));
	}

	// connects
	connect(ui->menuFileExit, SIGNAL(triggered()), this, SLOT(exit()));
	connect(ui->menuFileSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->menuFileImportMidi, SIGNAL(triggered()), this, SLOT(importMidi()));
	connect(ui->menuRackReset, SIGNAL(triggered()), this, SLOT(resetRack()));
	connect(ui->menuFileLoad, SIGNAL(triggered()), this, SLOT(load()));
	connect(ui->menuTracksClear, SIGNAL(triggered()), this, SLOT(clearTracks()));
}

MainWin::~MainWin() {
	delete ui;
}

void MainWin::exit() {
	ctx.getController()->exit();
}

void MainWin::load() {
	ctx.getController()->load();
}

void MainWin::save() {
	ctx.getController()->save();
}

void MainWin::importMidi() {
	ctx.getController()->importMidi();
}

void MainWin::resetRack() {
	ctx.getController()->resetRack();
}

void MainWin::clearTracks() {
	ctx.getController()->clearTracks();
}

void MainWin::addRackElement() {
	QObject* obj = sender();
	std::string val = obj->property("RackElement").toString().toStdString();
	ctx.getController()->addNewRackElement(val);
}
