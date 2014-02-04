#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
class Context;
#include "controls/misc/OSStats.h"

namespace Ui {
	class MainWin;
}

class MainWin : public QMainWindow {
	Q_OBJECT

public:

	/** ctor */
	explicit MainWin(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~MainWin();


private slots:

	void exit();

	void load();

	void save();

	void importMidi();

	void exportSong();

	void addRackElement();

	void resetRack();

	void clearTracks();

	void showSettings();

	void showAbout();

private:

	Ui::MainWin *ui;

	Context& ctx;

	OSStats* stats;

};

#endif // MAINWIN_H
