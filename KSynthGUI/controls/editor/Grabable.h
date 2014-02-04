#ifndef GRABABLE_H
#define GRABABLE_H

#include <QWidget>
#include <QMouseEvent>

enum class GrabStart {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	CENTER,
	NONE
};

/** the mouse must be moved at least by this amount to trigger events */
#define GRAB_MIN_PIXEL		3;

/**
 * this class provides grabbing (resizing/moving) for QWidgets
 * by capturing necessary mouse-events (press,release,move).
 */
class Grabable : public QWidget {

public:

	/** ctor */
	Grabable(QWidget* parent = 0) : QWidget(parent) {
		installEventFilter(this);
	}


	/** top/bottom grabbing allowed? */
	void setGrabY(bool allowed) {settings.grabY = allowed;}

	/** left/right grabbing allowed? */
	void setGrabX(bool allowed) {settings.grabX = allowed;}


	/** snap to multiples of X pixels */
	void setSnapX(unsigned int pixel) {if (pixel != 0) {settings.snapX = pixel;}}

	/** snap to multiples of Y pixels */
	void setSnapY(unsigned int pixel) {if (pixel != 0) {settings.snapY = pixel;}}


	/** capture mouse events */
	bool eventFilter(QObject* o, QEvent* e) {
		if (o != this) {return false;}
		if (e->type() == QEvent::MouseMove)				{return _mouseMove( (QMouseEvent*) e );}
		if (e->type() == QEvent::MouseButtonPress)		{return _mousePress( (QMouseEvent*) e ); }
		if (e->type() == QEvent::MouseButtonRelease)	{return _mouseRelease( (QMouseEvent*) e );}
		return false;
	}

private:

	/** mouse moving */
	bool _mouseMove(QMouseEvent* e) {

		// change cursor?
		if (!mouse.isDown) {
			GrabStart mg = mouseInGrab(e->pos());
			switch (mg) {
				case GrabStart::LEFT:
				case GrabStart::RIGHT:
					setCursor(Qt::SizeHorCursor);
					break;
				case GrabStart::TOP:
				case GrabStart::BOTTOM:
					setCursor(Qt::SizeVerCursor);
					break;
				default:
					setCursor(Qt::ArrowCursor);
					break;
			}
		}

		// resize or move ?
		if (mouse.isDown) {

			// check minimum for required mouse movement
			bool ok = std::abs(mouse.pos.x() - e->x()) + std::abs(mouse.pos.y() - e->y()) > GRAB_MIN_PIXEL;
			if (!ok) {return false;}

			QPoint diff = e->pos() - mouse.pos;
			diff.setY( diff.y() / settings.snapY * settings.snapY );
			diff.setX( diff.x() / settings.snapX * settings.snapX );

			QPoint pos = mapToParent(diff);

			if ( mouse.grab == GrabStart::LEFT ) {

				// resize from left (change x and width)
				mouse.newGeo.setRect( pos.x(), mouse.oldGeo.y(), width() - diff.x(), mouse.oldGeo.height() );

			} else if ( mouse.grab == GrabStart::RIGHT ) {

				// resize from right (change width)
				mouse.newGeo.setRect( mouse.oldGeo.x(), mouse.oldGeo.y(), mouse.oldGeo.width() + diff.x(), mouse.oldGeo.height() );

			} else if ( mouse.grab == GrabStart::CENTER ) {

				// move
				mouse.newGeo.setRect(pos.x(), pos.y(), mouse.oldGeo.width(), mouse.oldGeo.height());

			}

			onGrab( mouse.newGeo.x(), mouse.newGeo.y(), mouse.newGeo.width(), mouse.newGeo.height() );

			return true;

		}

		return false;

	}

	/** mouse-button pressed */
	bool _mousePress(QMouseEvent* e) {
		mouse.isDown = e->button() == Qt::LeftButton;
		mouse.pos = e->pos();
		mouse.grab = mouseInGrab(e->pos());
		mouse.oldGeo = geometry();
		mouse.newGeo = geometry();
		return true;
	}

	/** mouse-button released */
	bool _mouseRelease(QMouseEvent* e) {
		Q_UNUSED(e);
		if (mouse.isDown) {
			mouse.isDown = false;

			// check minimum for required mouse movement
			bool ok = std::abs(mouse.pos.x() - e->x()) + std::abs(mouse.pos.y() - e->y()) > GRAB_MIN_PIXEL;
			if (ok) {
				onGrabDone( mouse.newGeo.x(), mouse.newGeo.y(), mouse.newGeo.width(), mouse.newGeo.height() );
				return true;
			}

		}
		return false;
	}

	/** where was the element grabbed? (left/right/top/bottom) */
	GrabStart mouseInGrab(const QPoint& pos) {

		// left/right grabbing allowed?
		if (settings.grabX) {
			if (pos.x() <= settings.border) {return GrabStart::LEFT;}
			if (width() - pos.x() <= settings.border) {return GrabStart::RIGHT;}
		}

		// top/bottom grabbing allowed?
		if (settings.grabY) {
			if (pos.y() <= settings.border) {return GrabStart::TOP;}
			if (height() - pos.y() <= settings.border) {return GrabStart::BOTTOM;}
		}

		// center
		return GrabStart::CENTER;

	}


protected:

	/** result (while grabbing) */
	virtual void onGrab(int x, int y, int w, int h) = 0;

	/** result (after mouse release) */
	virtual void onGrabDone(int x, int y, int w, int h) = 0;


private:

	/** store the mouse state here */
	struct Mouse {

		/** is the mouse currently down? */
		bool isDown;

		/** grabbing started at .. */
		GrabStart grab;

		/** grabbing started here */
		QPoint pos;

		/** elements geometry at start */
		QRect oldGeo;

		/** elements new geometry */
		QRect newGeo;

		/** ctor */
		Mouse() : isDown(false), grab(GrabStart::NONE), pos(0,0) {;}

	} mouse;


	/** settings for the grabber */
	struct Settings {

		/** supports left/right grabbing */
		bool grabX;

		/** supports top/bottom grabbing */
		bool grabY;

		/** the border to allow grabbing */
		int border;

		/** snap to multiple of X pixels */
		int snapX;

		/** snap to multiple of Y pixels */
		int snapY;

		/** ctor */
		Settings() : grabX(true), grabY(true), border(3), snapX(1), snapY(1) {;}

	} settings;



};

#endif // GRABABLE_H
