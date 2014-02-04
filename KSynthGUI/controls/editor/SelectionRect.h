#ifndef SELECTIONRECT_H
#define SELECTIONRECT_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

#include <iostream>

/** listen for selection rectangles */
class SelectionRectListener {

public:

	/** the following widgets have been selected */
	virtual void onSelection(std::vector<QWidget*> list, QMouseEvent* e) = 0;

};



/**
 * draw selection rectangles within widgets
 */
class SelectionRect : public QObject {
	Q_OBJECT

public:

	/** ctor */
	SelectionRect(QWidget& parent, SelectionRectListener& listener) :
		parent(parent), listener(listener) {

	}

	/** enable/disable the selection rectangle */
	void setEnabled(bool enabled) {
		if (enabled) {
			parent.installEventFilter(this);
		} else {
			parent.removeEventFilter(this);
		}
	}

	/** draw the current selection rect using the provided QPainter */
	void draw(QPainter& p) {
		if (!sel.isDown) {return;}
		p.setPen(QColor(0,0,0));
		p.drawRect( sel.getRect() );
	}


protected:

	/** capture mouse events */
	bool eventFilter(QObject* o, QEvent* e) {
		if (o != &parent) {return false;}
		if (e->type() == QEvent::MouseMove)				{_mouseMove( (QMouseEvent*) e );}
		if (e->type() == QEvent::MouseButtonPress)		{_mousePress( (QMouseEvent*) e );}
		if (e->type() == QEvent::MouseButtonRelease)	{_mouseRelease( (QMouseEvent*) e );}
		return false;
	}

	void _mousePress(QMouseEvent* e) {
		sel.isDown = e->button() == Qt::LeftButton;
		if (sel.isDown) {
			sel.sx = e->x();
			sel.sy = e->y();
			sel.ex = sel.sx;
			sel.ey = sel.sy;
		}
	}

	void _mouseMove(QMouseEvent* e) {
		if (!sel.isDown) {return;}
		sel.ex = e->x();
		sel.ey = e->y();
		emit parent.repaint();
	}

	void _mouseRelease(QMouseEvent* e) {
		Q_UNUSED(e);
		if (!sel.isDown) {return;}
		sel.isDown = false;
		emit parent.repaint();
		getSelection(e);
	}

	void getSelection(QMouseEvent* e) {

		// find all childs within selection rectangle
		std::vector<QWidget*> vec;
		QRect rect = sel.getRect();
		for (QWidget* o : parent.findChildren<QWidget*>()) {
			if (rect.contains(o->geometry())) { vec.push_back(o); }
		}

		// call listener
		listener.onSelection(vec, e);

	}


private:

	/** the element to register to */
	QWidget& parent;

	/** called after selecting some widgets */
	SelectionRectListener& listener;

	/** current selection state/rectangle */
	struct Sel {

		/** left mouse button down? */
		bool isDown;

		/** start x coordinate */
		int sx;

		/** start y coordinate */
		int sy;

		/** end x coordinate */
		int ex;

		/** end y coordinate */
		int ey;

		/** ctor */
		Sel() : isDown(false), sx(0), sy(0), ex(0), ey(0) {;}

		/** get the selection as rectangle */
		QRect getRect() const {return QRect(sx, sy, ex-sx, ey-sy);}

	} sel;

};

#endif // SELECTIONRECT_H
