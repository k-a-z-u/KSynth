#ifndef RIGHTCLICKMENU_H
#define RIGHTCLICKMENU_H

#include <QMenu>
#include <memory>

/**
 * listen for context-menu requests
 * and add entries to the to-be-shown menu
 */
class RightClickMenuListener {

public:

	/** context-menu request: add entries to the given menu */
	virtual void onRightClickMenu(QMenu& menu) = 0;

};


/**
 * this class provides a (custom) right-click menu for any class
 * using this as a member variable.
 *
 * the menu provides listeners (called on right-click) to dynamically
 * create the menu structure.
 *
 * the listeners are handeled internally via std::unique_ptrs and thus
 * deleted on destruction.
 */
class RightClickMenu : QObject {
	Q_OBJECT

public:

	/** ctor */
	RightClickMenu(QWidget* parent);

	/**
	 * add a new listener for right-click-menu creator.
	 * the listener will be deleted automatically!!!
	 */
	void addRightClickMenuListener(RightClickMenuListener* l);


private slots:

	/** right-click -> show context menu */
	void showContextMenu(const QPoint& pt);


private:

	/** the listeners (for menu creation) are stored here and deleted on destruction */
	std::vector< std::unique_ptr<RightClickMenuListener> >listeners;

	/** the parent the context-menu belongs to */
	QWidget* parent;

};






#endif // RIGHTCLICKMENU_H
