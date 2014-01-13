#include "RackElement.h"

#include <KSynth/notes/NoteDevice.h>
#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>

#include "../model/Context.h"
#include "Rack.h"
#include "../controls/TextLabel.h"

#include "MasterTarget1.h"

RackElement::RackElement(Context &ctx, QWidget *parent) :
	QWidget(parent), ctx(ctx) {

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

	label = new TextLabel(this);
	connect(label, SIGNAL(valueChanged()), this, SLOT(updateUserName()));

}

RackElement::~RackElement() {
	;
}

#include <QMenu>
void RackElement::showContextMenu(const QPoint& pt) {
	QMenu menu;
	menu.addAction("delete", this, SLOT(deleteMe()));
	menu.exec(this->mapToGlobal(pt));
}

void RackElement::deleteMe() {
	ctx.getRack()->remove(this);
}

void RackElement::setSize(unsigned int width, unsigned int height) {
	setMinimumSize(width, height);
	setMaximumSize(width, height);
}

void RackElement::updateUserName() {
	setUserName(label->getText());
	emit userNameChanged();
}

void RackElement::setUserName(const std::string& name) {
	DeviceDetails::setUserName(name);
	label->setText(name);
}
