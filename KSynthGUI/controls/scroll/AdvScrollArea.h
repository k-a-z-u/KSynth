#ifndef ADVSCROLLAREA_H
#define ADVSCROLLAREA_H

#include <QWidget>
class QScrollBar;
class QVBoxLayout;
#include <QLayout>

#include <iostream>
class AdvLayout : public QLayout {
public:

	AdvLayout() {
		setSpacing(0);
		setMargin(0);
	}

	QSize sizeHint() const override {
		if (children().empty()) {return QSize(0,0);}
		QSize hint = ((QWidget*)children().at(0))->sizeHint();
//		std::cout << child << std::endl;
//		QSize hint = (child) ? (child->size()) : (QSize(0,0));
		std::cout << "HINT: " << hint.width() << ":" << hint.height() << std::endl;
		return hint;
	}
	void addItem(QLayoutItem* i) override {
		Q_UNUSED(i);
		;
	}
	QLayoutItem* itemAt(int i) const override {
		Q_UNUSED(i);
		return nullptr;
	}
	QLayoutItem* takeAt(int i) override {
		Q_UNUSED(i);
		return nullptr;
	}
	int count() const override {
		return children().size();
	}

	};

class AdvScrollArea : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	AdvScrollArea(QWidget* parent = 0);


	/** set the widget to scroll */
	void setWidget(QWidget* content);

	/** set the horizontal header */
	void setHeaderH(QWidget* widget, unsigned int height);

	/** set the vertical header */
	void setHeaderV(QWidget* widget, unsigned int width);

	/** add a new widget which is displayed on top of all other widgets */
	void addOverlayWidget(QWidget* overlay);


	/** get the width of the scrollable area */
	unsigned int getScrollAreaWidth() const;

	/** get the height of the scrollable area */
	unsigned int getScrollAreaHeight() const;


	/** get the number of scrolled X pixels */
	unsigned int getScrollX() const;

	/** get the number of scrolled Y pixels */
	unsigned int getScrollY() const;


	/** scroll x pixels (from the start) in x direction */
	void setScrollX(int x);

	/** scroll y pixels (from the start) in y direction */
	void setScrollY(int y);


	/** ensure the given x coordinate is visible */
	void ensureVisibleX(int x);

	/** ensure the given y coordinate is visible */
	void ensureVisibleY(int y);


protected:

	void resizeEvent(QResizeEvent* e) override;

	bool eventFilter(QObject* o, QEvent* e) override;

private:


	/** update scrollbars values */
	void updateScrollBars();

	/** move (scroll) the widget */
	void updateWidgetPosition();


	/** to-be-scrolled content goes here */
	QWidget* area;

	/** to-be-scrolled content */
	QWidget* content;

	QLayout* layout;

	/** vertical header (if any) */
	struct HV {
		QWidget* widget;
		unsigned int width;
		HV() : widget(nullptr), width(0) {;}
	} headerV;

	/** horizontal header (if any) */
	struct HH {
		QWidget* widget;
		unsigned int height;
		HH() : widget(nullptr), height(0) {;}
	} headerH;


	/** vertical scrolling */
	QScrollBar* scrollV;

	/** horizontal scrolling */
	QScrollBar* scrollH;


private slots:

	/** vertical scrolling */
	void onScrollV();

	/** horizontal scrolling */
	void onScrollH();

};

#endif // ADVSCROLLAREA_H
