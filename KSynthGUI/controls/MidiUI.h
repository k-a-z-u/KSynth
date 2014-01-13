#ifndef MIDIUI_H
#define MIDIUI_H

#include <KSynth/misc/DataTypes.h>
#include <QWidget>

class MidiUI : public QWidget {

public:

	/** ctor */
	explicit MidiUI(QWidget* parent = 0) : QWidget(parent) {;}

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

protected:

	/** the param index */
	Param param;


};

#endif // MIDIUI_H
