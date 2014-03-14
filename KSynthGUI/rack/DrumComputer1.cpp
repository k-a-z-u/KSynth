#include "DrumComputer1.h"

#include "../controls/CheckBox.h"
#include "../controls/CheckBoxGroup.h"
#include "../controls/ColorButton.h"
#include "../controls/Knob.h"
#include "../controls/VUMeter.h"
#include "../controls/Slider1.h"
#include "../controls/LED1.h"
#include "../controls/PinConnector.h"
#include "../controls/TextLabel.h"
#include "../controls/Image.h"
#include "../controls/NumberSpin.h"

#include "../misc/Skin.h"
#include "../misc/Helper.h"
#include "../model/Context.h"
#include <KSynth/Sequencer.h>
#include "../SampleChooserDialog.h"

#include <QPushButton>
#include <QLabel>
#include <QVariant>

#include <KSynth/tts/TTS.h>

DrumComputer1::DrumComputer1(Context& ctx, QWidget* parent) :
	RackElement(ctx, parent), curBank(0) {

	setSize(760, 216);

	elements.connector = new PinConnector(ctx, this, this);

	elements.spnPatternLength = new NumberSpin(this, 4, 32, 32);
	elements.spnPatternLength->connectTo( (int)SimpleDrumComputerParameter::PATTERN_LENGTH, this, SLOT(onParamChange()));

	elements.spnPatternBase = new NumberSpin(this, 1, 8, 4);
	elements.spnPatternBase->connectTo( (int)SimpleDrumComputerParameter::PATTERN_BASE, this, SLOT(onParamChange()));

	elements.grpPattern = new CheckBoxGroup();
	for (unsigned int i = 0; i < SDC_PATTERNS; ++i) {
		elements.chkPattern[i] = new CheckBox(this);
		elements.grpPattern->add(elements.chkPattern[i]);
		elements.chkPattern[i]->connectTo(i, this, SLOT(selectPattern()));
	}

	elements.grpBanks = new CheckBoxGroup();
	static QFont font1; font1.setPixelSize(8); font1.setBold(true);
	for (unsigned int i = 0; i < SDC_BANKS; ++i) {
		elements.banks[i].btnOpen = new QPushButton(this);
		elements.banks[i].btnOpen->setProperty("IDX", QVariant(i));
		connect(elements.banks[i].btnOpen, SIGNAL(clicked()), this, SLOT(openSample()));
		elements.banks[i].lblFile = new QLabel(this);
		elements.banks[i].lblFile->setFont(font1);
		elements.banks[i].lblFile->setStyleSheet("color:#ffffff;");
		elements.banks[i].chkEdit = new CheckBox(this);
		elements.grpBanks->add(elements.banks[i].chkEdit);
		elements.banks[i].chkEdit->connectTo(i, this, SLOT(selectBank()));
		elements.banks[i].kVol = new Knob("vol", 0, 127, 0, 5, this);
		elements.banks[i].kVol->connectTo((int)SimpleDrumComputerParameter::BANK1_VOLUME+i, this, SLOT(onParamChange()));
		elements.banks[i].kVol->addSnap(100, 5);
		elements.banks[i].kOffset = new Knob("offset", 0, 100, 0, 5, this);
		elements.banks[i].kOffset->connectTo((int)SimpleDrumComputerParameter::BANK1_START_OFFSET+i, this, SLOT(onParamChange()));
		elements.banks[i].ledBeat = new LED1(this);
	}

	for (unsigned int i = 0; i < SDC_PATTERN_MAX_LEN; ++i) {
		elements.chkBeat[i] = new ColorButton(this);
		elements.chkBeat[i]->connectTo(i, this, SLOT(selectBeat()));
	}

	elements.vu = new VUMeter(this);
	elements.sldVol = new Slider1("main", 0, 127, 0, 5, this);
	elements.sldVol->addSnap(100, 5);
	elements.sldVol->connectTo((int)SimpleDrumComputerParameter::MAIN_VOLUME, this, SLOT(onParamChange()));
	elements.memory.imgGui = new Image(this);

	std::string txt =
			"<voice gender='female'>Mary had a little lamb,</voice>";

	samples[0].s = TTS::get().toSample(txt);
	samples[0].s.convertTo(getSampleRate());

	samples[1].s = TTS::get().toSample("world");
	samples[1].s.convertTo(getSampleRate());

}


DrumComputer1::~DrumComputer1() {
	delete elements.grpBanks;
	delete elements.grpPattern;

}

void DrumComputer1::openSample() {

	QObject* caller = sender();
	const unsigned int idx = caller->property("IDX").toInt();
	const unsigned int paramIdx = (int)SimpleDrumComputerParameter::BANK1_FILE + idx;

	// currently selected sample (if any)
	std::string curFile = getParameterName(paramIdx);
	std::string newFile;

	// show chooser dialog
	if ( !curFile.empty() && K::File(curFile).getParent().isFolder() ) {
		newFile = SampleChooserDialog::openSampleFile("select new sample for slot " + std::to_string(idx+1), K::File(curFile), this);
	} else {
		newFile = SampleChooserDialog::openSampleFile("select new sample for slot " + std::to_string(idx+1), this);
	}

	// set new sample?
	if (newFile.empty()) {return;}
	setParameterName(paramIdx, newFile);

}


void DrumComputer1::onRackAttach() {
	// attach to sequencer
	ctx.getSequencer()->addBeatListener(this);
}

void DrumComputer1::onRackDetach() {
	// detach from sequencer
	ctx.getSequencer()->removeBeatListener(this);
}

void DrumComputer1::onParamChange() {

	// get the signal sender
	QObject* obj = QObject::sender();

	// cast it to a midi-ui element
	const MidiUI* ui = dynamic_cast<const MidiUI*>(obj);
	if (!ui) {throw "invalid element attached";}

	// get the new value to set (between [0,1])
	Param param = ui->getParamType();
	ParamValue val = ui->getValueAsParam();

	// set
	setParameter(param, val);

}

#include <QPainter>
void DrumComputer1::paintEvent(QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	static QImage imgBg = Skin::getImage("skin/drumcomputer1.png", "PNG");
	p.drawImage(0,0, imgBg);

}

void DrumComputer1::resizeEvent(QResizeEvent* event) {

	Q_UNUSED(event);

	elements.sldVol->setGeometry(662,122-8, 0,0);
	elements.vu->setGeometry(662+32,122, 0,0);

	elements.spnPatternBase->setGeometry(504+14,150, 0,0);
	elements.spnPatternLength->setGeometry(504,180, 0,0);
	elements.memory.imgGui->setGeometry(661, 21, 0,0);

	elements.connector->setGeometry(727+4,8, 0,0);
	label->setGeometry(723+4,45, 32,92);

	// pattern selection buttons
	unsigned int sx = 574;
	unsigned int sy = 142;
	unsigned int ssx = 16;
	unsigned int ssy = 16;
	for (unsigned int y = 0; y < 4; ++y) {
		for (unsigned int x = 0; x < 4; ++x) {
			unsigned int i = y * 4 + x;
			elements.chkPattern[i]->setGeometry(sx+ssx*x,sy+ssy*y, 0,0);
		}
	}

	// sample controls
	sx = 12;
	sy = 14;
	ssx = 160;
	ssy = 64;
	for (unsigned int x = 0; x < 4; ++x) {
		for (unsigned int y = 0; y < 2; ++y) {
			unsigned int i = y*4 + x;
			elements.banks[i].btnOpen->setGeometry(	sx+ssx*(x+1)-36,	sy+ssy*y-4,		20,16);
			elements.banks[i].lblFile->setGeometry(	sx+ssx*x+14,		sy+ssy*y-6,		96,16);
			elements.banks[i].chkEdit->setGeometry(	sx+ssx*x+8,			sy+ssy*y+32,	0,0);
			elements.banks[i].kVol->setGeometry(	sx+ssx*(x+1)-44,	sy+ssy*y+12,	0,0);
			elements.banks[i].kOffset->setGeometry(	sx+ssx*(x+1)-76,	sy+ssy*y+12,	0,0);
			elements.banks[i].ledBeat->setGeometry(	sx+ssx*x,			sy+ssy*y-4,		0,0);
		}
	}

	// editable color buttons
	sx = 14;
	ssx = 28;
	sy = 174-28;
	ssy = 28;
	for (unsigned int i = 0; i < SDC_PATTERN_MAX_LEN; ++i) {
		if (i == 16) {sy += ssy;}
		int j = i % 16;
		elements.chkBeat[i]->setGeometry(	sx+ssx*j+(j/4*5),	sy,			0,0);
	}

}

void DrumComputer1::selectPattern() {

	// get the signal sender
	QObject* obj = QObject::sender();

	// cast to ui element
	const MidiUI* ui = dynamic_cast<const MidiUI*>(obj);
	if (!ui) {throw "invalid element attached";}

	// only catch the event of the newly selected item
	if (ui->getValueAsParam() != 1.0f) {return;}

	// select the currently active pattern
	unsigned int pattern = ui->getParamType();
	setPattern(pattern);

	refresh();

}


void DrumComputer1::selectBank() {

	// get the signal sender
	QObject* obj = QObject::sender();

	// cast to ui element
	const MidiUI* ui = dynamic_cast<const MidiUI*>(obj);
	if (!ui) {throw "invalid element attached";}

	// select the active bank
	this->curBank = ui->getParamType();

	refresh();

}

#include <QKeyEvent>
void DrumComputer1::selectBeat() {

	// get the signal sender
	QObject* obj = QObject::sender();

	// cast to ui element
	const MidiUI* ui = dynamic_cast<const MidiUI*>(obj);
	if (!ui) {throw "invalid element attached";}

	// convert to bool
	unsigned int idx = ui->getParamType();
	bool isSet = getCurrentPattern()->getBank(curBank)->isSet(idx);

	Volume vol = 0.0f;
	if (!isSet) {
		ColorButton* cb = (ColorButton*) QObject::sender();
		vol = (cb->getLastEvent()->modifiers() == Qt::ShiftModifier) ? (0.5f) : (1.0f);
	}

	getCurrentPattern()->getBank(curBank)->set(idx, vol);

}

void DrumComputer1::refresh() {

	for (unsigned int i = 0; i < SDC_BANKS; ++i) {
		std::string name = getSample(i)->getName();
		std::string str = "none";
		if (!name.empty()) {str = K::File(name).getName();}
		if (elements.banks[i].lblFile->text() != str.c_str()) {
			elements.banks[i].lblFile->setText( str.c_str() );
		}
	}

	// beat selectors
	static const QColor c1(220,220,0);
	static const QColor c2(164,164,164);
	static const QColor c3(32,32,32);
	for (unsigned int i = 0; i < getCurrentPatternLength(); ++i) {
		//bool checked = getCurrentPattern()->getBank(curBank)->isSet(i);
		Volume vol = getCurrentPattern()->getBank(curBank)->get(i);
		QColor c = c2;
		if (vol > 0.0f) {
			c = c1;
			c.setAlpha( int(255*vol) );
		}
		if (getCurrentBeat() == i) {c = c.lighter();}
		elements.chkBeat[i]->setColor(c);
	}

	for (unsigned int i = getCurrentPatternLength(); i < SDC_PATTERN_MAX_LEN; ++i) {
		elements.chkBeat[i]->setColor(c3);
	}



	// beat LEDs
	for (unsigned int i = 0; i < SDC_BANKS; ++i) {
		elements.banks[i].ledBeat->setValue(  getCurrentPattern()->getBank(i)->isSet(getCurrentBeat()) );
	}

	// knobs / settings
	for (unsigned int i = 0; i < 8; ++i) {
		elements.banks[i].kVol->setValueFromParam(getParameter( (int)SimpleDrumComputerParameter::BANK1_VOLUME + i));
		elements.banks[i].kOffset->setValueFromParam(getParameter( (int)SimpleDrumComputerParameter::BANK1_START_OFFSET + i));
	}

	// misc
	elements.spnPatternBase->setValueFromParam(getParameter(SimpleDrumComputerParameter::PATTERN_BASE));
	elements.spnPatternLength->setValueFromParam(getParameter(SimpleDrumComputerParameter::PATTERN_LENGTH));
	elements.sldVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::MAIN_VOLUME));
	elements.vu->setValue(getVU());

	// memory usage
	if (elements.memory.used != getMemoryUsage()) {
		elements.memory.used = getMemoryUsage();
		unsigned int kb = elements.memory.used / 1024;
		elements.memory.imgGui->setImage( Helper::getForNumber(kb, 54) );
	}

}
