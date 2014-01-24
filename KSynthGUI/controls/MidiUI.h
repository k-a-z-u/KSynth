#ifndef MIDIUI_H
#define MIDIUI_H

#include <KSynth/misc/DataTypes.h>
#include <QWidget>
#include "RightClickMenu.h"
#include "../rack/RackElement.h"
#include <KSynth/misc/SoundBase.h>



/**
 * base class for all controls with "midi-flavour".
 *
 * those controls provide params within [0;1] and are
 * bound to a parameter (index) of the device from the
 * backend (e.g. synthesizer's master-volume).
 */
class MidiUI : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit MidiUI(QWidget* parent = 0);

	/** dtor */
	virtual ~MidiUI() {;}


	/** get the gui control's value as float between [0;1] */
	virtual ParamValue getValueAsParam() const = 0;

	/** set the gui control's value as float between [0;1] */
	virtual void setValueFromParam(ParamValue val) = 0;

	/** get the type of the parameter */
	Param getParamType() const {return param;}

	/** set the type of the parameter */
	void setParamType(Param param) {this->param = param;}

	/** convenience function to both: setParamType() and connect() */
	void connectTo(Param param, QObject* reciever, const char* slot) {
		setParamType(param);
		connect(this, SIGNAL(onChange()), reciever, slot);
	}


protected slots:

	void bindMidi();

	void unbindMidi();


protected:

	/** the param index */
	Param param;

	RightClickMenu rcMenu;


};


#endif // MIDIUI_H
