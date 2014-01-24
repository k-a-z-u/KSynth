#include "MainWin.h"
#include "ui_MainWin.h"

#include "model/Context.h"
#include "rack/RackFactory.h"
#include "controller/Controller.h"
#include "misc/Skin.h"
#include "misc/SystemVersion.h"

#include <QLabel>

MainWin::MainWin(Context& ctx, QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWin), ctx(ctx) {

	ui->setupUi(this);
	setWindowTitle("KSynth");

	setGeometry(0,0, 1200,600);

	// available rack elements
	for (std::string& str : ctx.getRackFactory()->getDevices()) {
		QAction* act = ui->menuRackAddElement->addAction(str.c_str());
		act->setProperty("RackElement", QVariant(str.c_str()));
		connect(act, SIGNAL(triggered()), this, SLOT(addRackElement()));
	}

	// connects
	connect(ui->menuFileExit, SIGNAL(triggered()), this, SLOT(exit()));
	connect(ui->menuFileSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->menuFileLoad, SIGNAL(triggered()), this, SLOT(load()));
	connect(ui->menuFileImportMidi, SIGNAL(triggered()), this, SLOT(importMidi()));
	connect(ui->menuFileSettings, SIGNAL(triggered()), this, SLOT(showSettings()));

	connect(ui->menuRackReset, SIGNAL(triggered()), this, SLOT(resetRack()));
	connect(ui->menuTracksClear, SIGNAL(triggered()), this, SLOT(clearTracks()));

	connect(ui->menuHelpAbout, SIGNAL(triggered()), this, SLOT(showAbout()));


	// stats
	stats = new OSStats();
	ui->statusBar->addWidget(stats);

	// apply icons from skin (better alternative???)
	ui->menuFileLoad->setIcon(Skin::getIcon("skin/icons/load16.png", "PNG"));
	ui->menuFileSave->setIcon(Skin::getIcon("skin/icons/save16.png", "PNG"));
	ui->menuFileImportMidi->setIcon(Skin::getIcon("skin/icons/note16.png", "PNG"));
	ui->menuFileExit->setIcon(Skin::getIcon("skin/icons/exit16.png", "PNG"));
	ui->menuFileExportSong->setIcon(Skin::getIcon("skin/icons/export16.png", "PNG"));

	ui->menuRackReset->setIcon(Skin::getIcon("skin/icons/clear16.png", "PNG"));
	ui->menuRackAddElement->setIcon(Skin::getIcon("skin/icons/add16.png", "PNG"));

	ui->menuTracksClear->setIcon(Skin::getIcon("skin/icons/clear16.png", "PNG"));

	ui->menuFileSettings->setIcon(Skin::getIcon("skin/icons/settings16.png", "PNG"));

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

void MainWin::showSettings() {
	ctx.getController()->showSettings();
}

void MainWin::importMidi() {
	ctx.getController()->importMidi();
}

void MainWin::resetRack() {
	ctx.getController()->resetRack();
}

#include <QDialog>
void MainWin::showAbout() {

	QDialog dlg;
	QGridLayout lay;
	QLabel lbl;

	std::string str = "<b>KSynth</b><br/><br/>";
	str += "<u>compiled features</u>";
	str += "<table>";
	str += "<tr><td width='75'>ALSA</td><td>";
	str += (SystemVersion::hasALSA() ? "yes" : "not compiled");
	str += "</td></tr> <tr><td>PulseAudio</td><td>";
	str += (SystemVersion::hasPulseAudio() ? "yes" : "not compiled");
	str += "</td></tr> <tr><td>WaveOut</td><td>";
	str += (SystemVersion::hasWaveOut() ? "yes" : "not compiled");
	str += "</td></tr> <tr><td>FLAC</td><td>";
	str += (SystemVersion::hasFLAC() ? "yes" : "not compiled");
	str += "</td></tr> <tr><td>FFTW3</td><td>";
	str += (SystemVersion::hasFFTW3() ? "yes" : "not compiled");
	str += "</td></tr> <tr><td>MP3 LAME</td><td>";
	str += (SystemVersion::hasMP3Lame() ? "yes" : "not compiled");
	str += "</table>";

	lbl.setText(str.c_str());

	lay.addWidget(&lbl);
	dlg.setLayout(&lay);
	dlg.exec();

}

void MainWin::clearTracks() {
	ctx.getController()->clearTracks();
}



void MainWin::addRackElement() {
	QObject* obj = sender();
	std::string val = obj->property("RackElement").toString().toUtf8().constData();
	ctx.getController()->addNewRackElement(val);
}
