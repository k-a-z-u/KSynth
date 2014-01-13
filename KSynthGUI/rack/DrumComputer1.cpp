#include "DrumComputer1.h"

#include "../controls/CheckBox.h"
#include "../controls/CheckBoxGroup.h"
#include "../controls/ColorButton.h"
#include "../controls/Knob.h"
#include "../controls/VUMeter.h"
#include "../controls/Slider1.h"
#include "../controls/LED1.h"
#include "../controls/PinConnector.h"

#include "../misc/Helper.h"
#include "../model/Context.h"
#include <KSynth/Sequencer.h>
#include "../SampleChooserDialog.h"

#include <QPushButton>
#include <QLabel>
#include <QVariant>

DrumComputer1::DrumComputer1(Context& ctx, QWidget* parent) :
	RackElement(ctx, parent), curBank(0) {

	setSize(760, 216);

	elements.connector = new PinConnector(ctx, this, this);

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
		elements.banks[i].kVol = new Knob("vol", 0, 127, 0, this);
		elements.banks[i].kVol->connectTo((int)SimpleDrumComputerParameter::BANK1_VOLUME+i, this, SLOT(onParamChange()));
		elements.banks[i].kVol->addSnap(100, 5);
		elements.banks[i].ledBeat = new LED1(this);
	}

	for (unsigned int i = 0; i < SDC_PATTERN_SIZE; ++i) {
		elements.chkBeat[i] = new ColorButton(this);
		elements.chkBeat[i]->connectTo(i, this, SLOT(selectBeat()));
	}

	elements.vu = new VUMeter(this);
	elements.sldVol = new Slider1("main", 0, 127, 0, this);
	elements.sldVol->addSnap(100, 5);
	elements.sldVol->connectTo((int)SimpleDrumComputerParameter::MAIN_VOLUME, this, SLOT(onParamChange()));
	elements.memory.lbl = new QLabel(this);

//	K::File f1("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Mb_K1.flac");
//	K::File f2("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Kicks/Bnk_K.wav");

//	K::File f3("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Aki_H2.wav");
//	K::File f4("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Snares/Grov_Sn.wav");

//	K::File f5("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Ok_H.wav");
//	K::File f6("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Hi-Hats/Dol_H2.wav");

//	K::File f7("/apps/workspaces/synth/KSynth/samples/drumkits/FreeDrumKits.net - 9th Wonder Kit/Cymbals/Lod_Crsh.wav");

//	loadSample(f1, 0);
//	loadSample(f2, 1);
//	loadSample(f3, 2);
//	loadSample(f4, 3);
//	loadSample(f5, 4);
//	loadSample(f6, 5);
//	loadSample(f7, 6);
//	//loadSample(f8, 7);

}


DrumComputer1::~DrumComputer1() {
	delete elements.grpBanks;
	delete elements.grpPattern;

}

void DrumComputer1::openSample() {

	QObject* caller = sender();
	unsigned int idx = caller->property("IDX").toInt();

	// currently selected sample (if any)
	std::string curFile = getParameterName(idx);

	// show chooser dialog
	std::string file = SampleChooserDialog::openSampleFile("select new sample for slot " + std::to_string(idx+1), this);

	// set new sample?
	if (file.empty()) {return;}
	setParameterName( (int)SimpleDrumComputerParameter::BANK1_FILE + idx, file);

}


void DrumComputer1::onRackAttach() {
	// attach to sequencer
	ctx.getSequencer()->bind(this);
}

void DrumComputer1::onRackDetach() {
	// detach from sequencer
	ctx.getSequencer()->unbind(this);
}


//RackElement* DrumComputer1::createFromPrototype(Context& ctx) const {
//	DrumComputer1* dc1 = new DrumComputer1( (QWidget*) parent() );
//	ctx.seq->bind(dc1);
//	return dc1;
//}

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

	static QImage imgBg = Helper::getSkinImage("skin/drumcomputer1.png", "PNG");
	p.drawImage(0,0, imgBg);

}

//std::string DrumComputer1::getDeviceType() const {
//	return "Drumcomputer1";
//}

void DrumComputer1::resizeEvent(QResizeEvent* event) {

	Q_UNUSED(event);

	elements.sldVol->setGeometry(694,124-8, 0,0);
	elements.vu->setGeometry(694+32,124, 0,0);

	elements.memory.lbl->setGeometry(694, 16, 48, 32);

	elements.connector->setGeometry(670, 48, 0,0);

	// pattern selection buttons
	unsigned int sx = 540;
	unsigned int sy = 172;
	unsigned int ssx = 16;
	unsigned int ssy = 16;
	for (unsigned int y = 0; y < 2; ++y) {
		for (unsigned int x = 0; x < 8; ++x) {
			unsigned int i = y * 8 + x;
			elements.chkPattern[i]->setGeometry(sx+ssx*x,sy+ssy*y, 0,0);
		}
	}


	// sample controls
	sx = 12;
	sy = 14;
	ssx = 168;
	ssy = 64;
	for (unsigned int x = 0; x < 4; ++x) {
		for (unsigned int y = 0; y < 2; ++y) {
			unsigned int i = y*4 + x;
			elements.banks[i].btnOpen->setGeometry(	sx+ssx*x,			sy+ssy*y+4,		20,16);
			elements.banks[i].lblFile->setGeometry(	sx+ssx*x+24,		sy+ssy*y+5,		96,16);
			elements.banks[i].chkEdit->setGeometry(	sx+ssx*x,			sy+ssy*y+32,	0,0);
			elements.banks[i].kVol->setGeometry(	sx+ssx*(x+1)-48,	sy+ssy*y,		0,0);
			elements.banks[i].ledBeat->setGeometry(	sx+ssx*x-4,			sy+ssy*y-7,	0,0);
		}
	}

	// editable color buttons
	sx = 14;
	ssx = 28;
	sy = 174-28;
	ssy = 28;
	for (unsigned int i = 0; i < SDC_PATTERN_SIZE; ++i) {
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

void DrumComputer1::selectBeat() {

	// get the signal sender
	QObject* obj = QObject::sender();

	// cast to ui element
	const MidiUI* ui = dynamic_cast<const MidiUI*>(obj);
	if (!ui) {throw "invalid element attached";}

	// convert to bool
	unsigned int idx = ui->getParamType();
	//bool sel = ui->getValueAsParam().asBool();
	bool sel = getCurrentPattern()->getBank(curBank)->get(idx);
	getCurrentPattern()->getBank(curBank)->set(idx, !sel);

}

void DrumComputer1::refresh() {

	for (unsigned int i = 0; i < SDC_BANKS; ++i) {
		elements.banks[i].lblFile->setText( K::File(getSample(i)->getName()).getName().c_str() );
	}

	QColor c1(220,220,0);
	QColor c2(164,164,164);
	for (unsigned int i = 0; i < SDC_PATTERN_SIZE; ++i) {
		bool checked = getCurrentPattern()->getBank(curBank)->get(i);
		QColor c = (checked) ? c1 : c2;
		if (getCurrentBeat() == i) {c = c.lighter();}
		elements.chkBeat[i]->setColor(c);
	}

	// beat LEDs
	for (unsigned int i = 0; i < SDC_BANKS; ++i) {
		elements.banks[i].ledBeat->setValue(  getCurrentPattern()->getBank(i)->get(getCurrentBeat()) );
	}

	elements.banks[0].kVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::BANK1_VOLUME));
	elements.banks[1].kVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::BANK2_VOLUME));
	elements.banks[2].kVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::BANK3_VOLUME));
	elements.banks[3].kVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::BANK4_VOLUME));
	elements.banks[4].kVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::BANK5_VOLUME));
	elements.banks[5].kVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::BANK6_VOLUME));
	elements.banks[6].kVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::BANK7_VOLUME));
	elements.banks[7].kVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::BANK8_VOLUME));

	elements.sldVol->setValueFromParam(getParameter(SimpleDrumComputerParameter::MAIN_VOLUME));

	elements.vu->setValue(getVU());

	if (elements.memory.used != getMemoryUsage()) {
		elements.memory.used = getMemoryUsage();
		unsigned int kb = elements.memory.used / 1024;
		elements.memory.img = Helper::getForNumber(kb, 48, 22);
		elements.memory.lbl->setPixmap(QPixmap::fromImage(elements.memory.img));
	}

}
