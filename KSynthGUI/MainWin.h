#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
class Context;

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

	void addRackElement();

	void resetRack();

	void clearTracks();

private:

	Ui::MainWin *ui;

	Context& ctx;

};

#endif // MAINWIN_H
