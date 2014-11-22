#ifndef RACKWIDGET_H
#define RACKWIDGET_H

/**
  * this is the GUI representation of the Rack
  * containing all synths, mixers, fx, etc. as sub-widgets
  */

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


	/** move the given rack-element up by one position */
	void moveUp(RackElement* e);

	/** move the given rack-element down by one position */
	void moveDown(RackElement* e);


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
