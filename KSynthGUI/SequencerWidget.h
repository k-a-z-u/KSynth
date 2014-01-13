#ifndef SEQUENCERWIDGET_H
#define SEQUENCERWIDGET_H

#include <QDockWidget>

class Context;

namespace Ui {
	class SequencerWidget;
}

class SequencerWidget : public QDockWidget {
	Q_OBJECT


public:

	/** ctor */
	explicit SequencerWidget(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~SequencerWidget();


private:

	Ui::SequencerWidget *ui;

	Context& ctx;

};

#endif // SEQUENCERWIDGET_H
