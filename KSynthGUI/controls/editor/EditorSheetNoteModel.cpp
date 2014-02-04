
#include "EditorSheetNoteModel.h"

EditorSheetNoteModel::EditorSheetNoteModel() : on(nullptr), off(nullptr) {
	;
}

EditorSheetNoteModel::EditorSheetNoteModel(MidiEvent* on) : on(on), off(nullptr) {
	;
}

EditorSheetNoteModel::EditorSheetNoteModel(MidiEvent* on, MidiEvent* off) : on(on), off(off) {
	;
}

EditorSheetNoteModel::EditorSheetNoteModel(const EditorSheetNoteModel& other) {
	this->on = other.on;
	this->off = other.off;
}

unsigned int EditorSheetNoteModel::getLength() const {
	return off->getDelay() - on->getDelay();
}

bool EditorSheetNoteModel::isValid() const {
	return (on != nullptr) && (off != nullptr);
}

void EditorSheetNoteModel::reset() {
	on = nullptr;
	off = nullptr;
}

unsigned int EditorSheetNoteModel::getNoteNumber() const {
	return on->getData1();
}

const MidiEvent* EditorSheetNoteModel::getOnEvt() const {
	return on;
}

const MidiEvent* EditorSheetNoteModel::getOffEvt() const {
	return off;
}

void EditorSheetNoteModel::setOffEvt(MidiEvent* evt) {
	this->off = evt;
}

void EditorSheetNoteModel::setNoteNumber(unsigned int noteNumber) {
	on->setData1(noteNumber);
	off->setData1(noteNumber);
	emit changed();
}

void EditorSheetNoteModel::setTimes(TimeBase128 on, TimeBase128 off, unsigned int absGran) {

	unsigned int absDiv = 128 / absGran;

	// snap result (=absolute) to a multiple of the requested granularity
	TimeBase128 nOn = on / absDiv * absDiv;
	TimeBase128 nOff = off / absDiv * absDiv;

	// apply
	this->on->setDelay(nOn);
	this->off->setDelay(nOff);
	emit changed();

}

void EditorSheetNoteModel::moveTime(TimeOffset128 offset, unsigned int relGran, unsigned int absGran) {

	unsigned int relDiv = 128 / relGran;

	// snap offset (=relative) to a multiple of the requested granularity
	offset = int( ceil(offset / float(relDiv)) ) * relDiv;

	// set new times
	this->setTimes( on->delay + offset, off->delay + offset, absGran );
	emit changed();

}

void EditorSheetNoteModel::resizeTime(TimeOffset128 offset, unsigned int relGran, unsigned int absGran) {

	unsigned int absDiv = 128 / absGran;
	unsigned int relDiv = 128 / relGran;

	// snap offset (=relative) to a multiple of the requested granularity
	offset = int( ceil(offset / float(relDiv)) ) * relDiv;

	TimeBase128 nOff = (off->getDelay() + offset) / absDiv * absDiv;

	// set new time for "off"
	off->delay = nOff;
	emit changed();

}
