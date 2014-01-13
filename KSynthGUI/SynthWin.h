#ifndef SYNTHWIN_H
#define SYNTHWIN_H

#include <QMainWindow>
#include "model/Context.h"

namespace Ui {
	class SynthWin;
}

class SynthWin : public QMainWindow {
	Q_OBJECT

public:
	explicit SynthWin(Context& ctx, QWidget *parent = 0);
	~SynthWin();

private:

	Ui::SynthWin *ui;

	Context& ctx;

};

#endif // SYNTHWIN_H
