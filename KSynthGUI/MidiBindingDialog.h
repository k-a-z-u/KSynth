#ifndef MIDIBINDINGDIALOG_H
#define MIDIBINDINGDIALOG_H

#include <QDialog>

class Context;

#include <KSynth/midi/rt/RtMidiEventListener.h>
#include <KSynth/midi/MidiBinding.h>

class RackElement;
struct MidiBinding;

namespace Ui {
	class MidiBindingDialog;
}

class MidiBindingDialog : public QDialog, RtMidiEventListener {
	Q_OBJECT

public:

	/** ctor */
	explicit MidiBindingDialog(Context& ctx, QWidget *parent = 0);

	/** dtor */
	~MidiBindingDialog();

	/** get binding */
	bool get(RackElement* re, Param p, MidiBinding& b);


protected:

	void onMidiEvent(RtMidiInDevice& src, MidiEvent evt) override;


private:




private:

	Ui::MidiBindingDialog *ui;
	Context& ctx;

	/** the binding to fill with parameters */
	MidiBinding* binding;


	/** ok-ed or canceled? */
	bool okClicked;

private slots:

	/** ok button clicked */
	void onOk();

	/** cancel button clicked */
	void onCancel();



};

#endif // MIDIBINDINGDIALOG_H
