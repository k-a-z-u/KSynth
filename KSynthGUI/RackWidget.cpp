/**
  * this is the GUI representation of the Rack
  * containing all synths, mixers, fx, etc. as sub-widgets
  */

#include "RackWidget.h"
#include "ui_RackWidget.h"

#include "rack/RackElement.h"
#include "model/Context.h"

#include <QVBoxLayout>
//#include <QListWidget>

RackWidget::RackWidget(Context& ctx, QWidget *parent) :
	QDockWidget(parent), ui(new Ui::RackWidget), ctx(ctx) {

	ui->setupUi(this);

	// sizing
	setMinimumWidth(760+20);
	setMaximumWidth(760+20);

//	QVBoxLayout* lay0 = new QVBoxLayout();
//	lay0->setSpacing(1);
//	lay0->setMargin(0);
//	ui->scrollArea->setLayout(lay0);

	QWidget* w = new QWidget(nullptr);
	//w = new QListWidget(nullptr);
	lay = new QVBoxLayout(w);
	lay->setSpacing(2);
	lay->setMargin(0);
//	w->setLayout(lay);
	ui->scrollArea->setWidget(w);

	//QVBoxLayout* layScroll = new QVBoxLayout(ui->scrollArea);
	//layScroll->addWidget(w);
	ui->scrollArea->setStyleSheet("QScrollArea > QWidget {background-color: black;}");

}

RackWidget::~RackWidget() {
	//lay->setParent(nullptr);
	//ui->scrollArea->setWidget(nullptr);
	delete ui;
}

void RackWidget::add(RackElement* e) {
	lay->addWidget(e);
}

void RackWidget::moveUp(RackElement* e) {
	int pos = lay->indexOf(e);
	if (pos == 0) {return;}
	lay->removeWidget(e);
	lay->insertWidget(pos-1, e);
}

void RackWidget::moveDown(RackElement* e) {
	int pos = lay->indexOf(e);
	if (pos == lay->count() -1) {return;}
	lay->removeWidget(e);
	lay->insertWidget(pos+1, e);
}

void RackWidget::remove(RackElement* e) {
	e->setParent(nullptr);
}
