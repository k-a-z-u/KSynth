#include "MidiUI.h"
#include "RightClickMenu.h"
#include "../misc/Skin.h"
#include "../model/Context.h"
#include "MidiBindingDialog.h"
#include "KSynth/midi/MidiBinder.h"

/** -------- context-menu for midi controls -------- */

/** context-menu for midi controls */
class RightClickMenuMIDI : public RightClickMenuListener {

public:

	RightClickMenuMIDI(MidiUI* ui, RackElement* re) : ui(ui), re(re) {;}

	void onRightClickMenu(QMenu& menu) override {

		SoundBase* sb = dynamic_cast<SoundBase*>(re);
		if (!re || !sb) {return;}

		// info string within the menu
		std::string str = re->getProductString() + ": "+ re->getUserName() + ": " + sb->getParameterName(ui->getParamType());
		menu.addAction( str.c_str(), 0, 0)->setDisabled(true);
		menu.addSeparator();

		// menu entry for midi event binding (the rack element is attached as property)
		QAction* aAdd = menu.addAction("bind MIDI device", ui, SLOT(bindMidi()));
		aAdd->setIcon(Skin::getIcon("skin/icons/connect.png"));
		aAdd->setProperty("CONTEXT_MENU_RACK_ELEMENT", qVariantFromValue( (void*) re ));

		// menu entry for midi event unbinding (the rack element is attached as property)
		QAction* aDel = menu.addAction("unbind MIDI device", ui, SLOT(unbindMidi()));
		aDel->setIcon(Skin::getIcon("skin/icons/delete.png"));
		aDel->setProperty("CONTEXT_MENU_RACK_ELEMENT", qVariantFromValue( (void*) re ));

		bool has = re->getContext().getMidiBinder()->hasBinding(sb, ui->getParamType());
		aDel->setEnabled(has);
		aAdd->setEnabled(!has);

	}

private:

	MidiUI* ui;

	RackElement* re;

};




MidiUI::MidiUI(QWidget* parent) : QWidget(parent), rcMenu(this) {
	rcMenu.addRightClickMenuListener(
				new RightClickMenuMIDI(this, dynamic_cast<RackElement*>(parent))
				);
}

void MidiUI::bindMidi() {

	// get the rack element this control (slider, knob, ...) belongs to
	QObject* obj =  sender();
	RackElement* re = (RackElement*) obj->property("CONTEXT_MENU_RACK_ELEMENT").value<void*>();

	// as this element is a MidiUI element, it provides a Parameter it belongs to
	Param p = this->param;

	// create binding
	MidiBinding mb;
	bool ok = re->getContext().getMidiBindingDialog()->get(re, p, mb);

	// active binding?
	if (ok) {re->getContext().getMidiBinder()->addBinding(mb);}

}

void MidiUI::unbindMidi() {
	;
}
