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
		;
	}
	QLayoutItem* itemAt(int i) const override {
		return nullptr;
	}
	QLayoutItem* takeAt(int i) override {
		return nullptr;
	}
	int count() const override {
		return children().size();
	}

private:


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
