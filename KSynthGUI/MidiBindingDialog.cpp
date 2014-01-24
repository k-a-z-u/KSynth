#include "MidiBindingDialog.h"
#include "ui_MidiBindingDialog.h"

#include "model/Context.h"
#include "rack/RackElement.h"

#include <KSynth/midi/rt/RtMidiWrapper.h>
#include <KSynth/misc/SoundBase.h>


MidiBindingDialog::MidiBindingDialog(Context& ctx, QWidget *parent) :
	QDialog(parent), ui(new Ui::MidiBindingDialog), ctx(ctx), binding(nullptr), okClicked(false) {

	ui->setupUi(this);

	// register for midi events
	ctx.getMidi()->addListener(this);

	// buttons
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
	connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(onOk()));



}

MidiBindingDialog::~MidiBindingDialog() {

	delete ui;

	// deregister for midi events
	ctx.getMidi()->removeListener(this);

}

bool MidiBindingDialog::get(RackElement* re, Param p, MidiBinding& b) {

	SoundBase* sb = dynamic_cast<SoundBase*>(re);
	if (!sb) {return false;}

	// clear binding source (just to be sure it is cleared
	b.reset();

	// reset UI
	ui->lblSrcChan->setText("-");
	ui->lblSrcData->setText("-");
	ui->lblSrcDev->setText("-");
	ui->lblSrcEvt->setText("-");

	// display info
	ui->lblDstDev->setText( re->getUserName().c_str() );
	ui->lblDstCtrl->setText( sb->getParameterName(p).c_str() );

	// setup bindings destination
	b.dst.dev = sb;
	b.dst.param = p;

	// fill missing parameters
	this->binding = &b;
	exec();

	// ok or not ok?
	return okClicked;

}

void MidiBindingDialog::onMidiEvent(RtMidiInDevice& src, MidiEvent evt) {

	// display info
	ui->lblSrcDev->setText( src.getName().c_str() );
	ui->lblSrcEvt->setText( evt.getTypeString().c_str() );
	ui->lblSrcChan->setText( std::to_string(evt.getChannel()).c_str() );
	ui->lblSrcData->setText( ("d1=" + std::to_string(evt.getData1()) + " d2=" + std::to_string(evt.getData2())).c_str() );

	// setup bindings source
	if (!binding) {return;}
	binding->src.srcName = src.getName();
	binding->src.type = evt.getType();
	binding->src.channel = evt.getChannel();
	binding->src.d1 = evt.getData1();

}

#include <QMessageBox>
void MidiBindingDialog::onOk() {

	// ensure we found a proper source device!
	if (binding->src.srcName.empty()) {
		QMessageBox::warning(this, "invalid binding", "please select a source device first!");
		return;
	}

	// unbind the to-be-filled binding
	binding = nullptr;

	// done
	okClicked = true;
	emit close();

}

void MidiBindingDialog::onCancel() {


	// unbind the to-be-filled binding
	binding = nullptr;

	// done
	okClicked = false;
	emit close();

}
