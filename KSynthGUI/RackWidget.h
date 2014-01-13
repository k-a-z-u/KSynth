#ifndef RACKWIDGET_H
#define RACKWIDGET_H

#include <QDockWidget>

class Context;
class RackElement;
class QVBoxLayout;

namespace Ui {
	class RackWidget;
}

class RackWidget : public QDockWidget {
	Q_OBJECT


public:

	/** ctor */
	explicit RackWidget(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~RackWidget();

private:

	/** only the Rack is allowed to add entries here */
	friend class Rack;

	/** add a new element to the rack */
	void add(RackElement* e);

	/** remove an existing element form the rack */
	void remove(RackElement* e);

private:

	Ui::RackWidget *ui;

	Context& ctx;

	QVBoxLayout* lay;

};

#endif // RACKWIDGET_H
