#include "AdvScrollArea.h"
#include <QScrollBar>
#include <QVBoxLayout>
#include <QScrollArea>


AdvScrollArea::AdvScrollArea(QWidget* parent) :
	QWidget(parent), content(nullptr) {

	// both scrollbars
	scrollV = new QScrollBar(Qt::Vertical, this);
	scrollH = new QScrollBar(Qt::Horizontal, this);

	// connect events
	connect(scrollV, SIGNAL(valueChanged(int)), this, SLOT(onScrollV()));
	connect(scrollH, SIGNAL(valueChanged(int)), this, SLOT(onScrollH()));

	// the widget accepting the content
	area = new QWidget(this);

	updateScrollBars();
	updateWidgetPosition();

}

void AdvScrollArea::onScrollV() {
	updateWidgetPosition();
}

void AdvScrollArea::onScrollH() {
	updateWidgetPosition();
}

void AdvScrollArea::updateScrollBars() {

	// resize child to desired size
	QSize newSize = size();
	if (content) {newSize = content->layout()->sizeHint(); content->resize(newSize);}

	// update horizontal header's size
	if (headerH.widget) {headerH.widget->resize(newSize.width(), headerH.height);}

	// update vertical header's size
	if (headerV.widget) {headerV.widget->resize(headerV.width, newSize.height());}

	// update scrollbar maximum values
	// (adjusts to childs size minus scrolling area size)
	int maxH = newSize.width() - area->width();
	int maxV = newSize.height() - area->height();
	if (maxH < 0) {maxH = 0;}
	if (maxV < 0) {maxV = 0;}
	scrollV->setMaximum(maxV);
	scrollH->setMaximum(maxH);

}

void AdvScrollArea::updateWidgetPosition() {

	unsigned int top = (headerH.widget) ? (headerH.height) : (0);
	unsigned int left = (headerV.widget) ? (headerV.width) : (0);

	// move widget
	if (content)		{content->move(-scrollH->value(), -scrollV->value());}

	// move horizontal header
	if (headerH.widget)	{headerH.widget->move(-scrollH->value()+left, 0);}

	// move vertical header
	if (headerV.widget)	{headerV.widget->move(0, -scrollV->value()+top);}

}

void AdvScrollArea::setWidget(QWidget* content) {
	this->content = content;
	//layout->addWidget(content);
	content->setParent(area);
	content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	content->setMinimumSize(100, 100);
	content->move(0,0);
	content->installEventFilter(this);
	updateScrollBars();
}

void AdvScrollArea::setHeaderH(QWidget* widget, unsigned int height) {
	headerH.widget = widget;
	headerH.height = height;
	if (widget) {widget->setParent(this);}
}

void AdvScrollArea::setHeaderV(QWidget* widget, unsigned int width) {
	headerV.widget = widget;
	headerV.width = width;
	if (widget) {widget->setParent(this);}
}

#include <QEvent>
bool AdvScrollArea::eventFilter(QObject* o, QEvent* e) {

	//	if (d->widget && o != d->widget && e->type() == QEvent::FocusIn && QApplication::keypadNavigationEnabled()) {
	//		if (o->isWidgetType())
	//			ensureWidgetVisible(static_cast<QWidget *>(o));
	//	}

	// child resized -> adjust scrollbars
	if (o == content && e->type() == QEvent::Resize) {
		updateScrollBars();
	}

	return false;

}

#include <iostream>
#include <QResizeEvent>
#include <QSize>
void AdvScrollArea::resizeEvent(QResizeEvent* e) {

	std::cout << "RESIZE TO: " << e->size().width() << ":"<< e->size().height() << std::endl;

	Q_UNUSED(e);

	// scrollbar size
	unsigned int scrS = 16;
	unsigned int top = (headerH.widget) ? (headerH.height) : (0);
	unsigned int left = (headerV.widget) ? (headerV.width) : (0);

	// area
	unsigned int areaW = width() - scrS - left;
	unsigned int areaH = height() - scrS - top;
	area->setGeometry(left, top, areaW, areaH);
	area->setMaximumSize(areaW, areaH);

	// scrollbar position
	scrollV->setGeometry(width()-scrS, top, scrS, height()-scrS-top);
	scrollH->setGeometry(left, height()-scrS, width()-scrS-left, scrS);

	// scrollbar values
	updateScrollBars();

	// move the widget
	updateWidgetPosition();

}
