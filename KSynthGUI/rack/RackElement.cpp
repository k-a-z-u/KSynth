#include "RackElement.h"

#include <KSynth/notes/NoteDevice.h>
#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>

#include "../model/Context.h"
#include "Rack.h"
#include "RackWidget.h"
#include "../controls/TextLabel.h"
#include "../misc/Skin.h"

#include "MasterTarget1.h"

class RightClickMenuRackElement : public RightClickMenuListener {
public:
	RightClickMenuRackElement(RackElement* re) : re(re) {;}
	void onRightClickMenu(QMenu& menu) override {

		SoundBase* sb = dynamic_cast<SoundBase*>(re);
		if (!re || !sb) {return;}
		menu.addAction( (re->getProductString() + ": "+ re->getUserName()).c_str(), 0, 0)->setDisabled(true);
		menu.addSeparator();

		// delete this rack element
		QAction* actDel = menu.addAction("delete", re, SLOT(deleteMe()));
		actDel->setIcon(Skin::getIcon("skin/icons/delete.png"));

		// move this rack-element up by one position
		menu.addAction("move up", re, SLOT(moveUp()));

		// move this rack-element down by one position
		menu.addAction("move down", re, SLOT(moveDown()));

	}
private:
	RackElement* re;
};



RackElement::RackElement(Context &ctx, QWidget *parent) :
	QWidget(parent), ctx(ctx), rcMenu(this) {

	// right-click menu
	rcMenu.addRightClickMenuListener(new RightClickMenuRackElement(this));

	// every rack-element has a label which allows changing its name
	label = new TextLabel(this);
	connect(label, SIGNAL(valueChanged()), this, SLOT(updateUserName()));

}

RackElement::~RackElement() {
	;
}

Context& RackElement::getContext() {
	return ctx;
}

void RackElement::deleteMe() {
	ctx.getRack()->remove(this);
}

void RackElement::moveUp() {
	ctx.getRack()->moveUp(this);
}

void RackElement::moveDown() {
	ctx.getRack()->moveDown(this);
}

void RackElement::setSize(unsigned int width, unsigned int height) {
	setMinimumSize( int(width), int(height));
	setMaximumSize( int(width), int(height));
}

void RackElement::updateUserName() {
	setUserName(label->getText());
	emit userNameChanged();
}

void RackElement::setUserName(const std::string& name) {
	DeviceDetails::setUserName(name);
	label->setText(name);
}
