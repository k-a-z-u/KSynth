#include "RightClickMenu.h"

RightClickMenu::RightClickMenu(QWidget* parent) : parent(parent) {

	// custom context menu on right-click
	parent->setContextMenuPolicy(Qt::CustomContextMenu);
	parent->connect(parent, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

}

void RightClickMenu::addRightClickMenuListener(RightClickMenuListener* l) {

	// all listeners will be kept internally for proper deletion
	std::unique_ptr<RightClickMenuListener> ptr(l);
	listeners.push_back( std::move(ptr) );

}

void RightClickMenu::showContextMenu(const QPoint& pt) {

	// create menu
	QMenu menu;

	// fill by calling every listener
	for ( auto& l : listeners ) {l->onRightClickMenu(menu);}

	// show
	menu.exec(parent->mapToGlobal(pt));

}


