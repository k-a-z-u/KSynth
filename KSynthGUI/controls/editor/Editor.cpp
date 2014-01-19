#include "Editor.h"
#include "EditorTimeline.h"
#include "EditorSheet.h"
#include "EditorSheetHeader.h"
#include "EditorSlider.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QScrollBar>
#include <QWidget>

#include "../../model/Context.h"
#include "../../controls/SequencerTracksWidget.h"
#include "../../misc/Helper.h"

#include <KSynth/Sequencer.h>
#include <KSynth/midi/MidiEvent.h>


Editor::Editor(Context& ctx, SequencerTracksWidget& tracks, QWidget *parent) :
	QWidget(parent), ctx(ctx), tracks(tracks), mode(EditorMode::CURSOR) {

	// layout for the content
	layout = new QVBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);
	setLayout(layout);

	// layout for the vertical headers (left side)
	layoutHeader = new QVBoxLayout();
	layoutHeader->setMargin(0);
	layoutHeader->setSpacing(0);

	// widget for the left header
	header = new QWidget();
	header->setLayout(layoutHeader);

	connect(&tracks, SIGNAL(onCurrentItemChanged(SequencerTrack*)), this, SLOT(onTrackSelectionChanged(SequencerTrack*)));

	sheet = nullptr;
	sheetHeader = nullptr;

	// register for playing events
	ctx.getSequencer()->addBeatListener(this);
	slider = new EditorSlider(*this, ctx, this);
	slider->raise();

}

Editor::~Editor() {

	// unregister
	ctx.getSequencer()->removeBeatListener(this);

}

void Editor::setMode(EditorMode mode) {
	this->mode = mode;
	switch (mode) {
		case EditorMode::DRAW:		setCursor(Helper::getSkinCursor("skin/icons/pen.png", "PNG", 0, 0)); break;
		case EditorMode::CURSOR:	setCursor(Qt::ArrowCursor); break;
	}
}

EditorMode Editor::getMode() const {
	return mode;
}

EditorSlider* Editor::getSlider() const {
	return slider;
}

Context& Editor::getContext() const {
	return ctx;
}

void Editor::onBeat(Beat beat, Time time) {
	Q_UNUSED(time);
	static unsigned int cnt = 0;
	if (++cnt % 10 != 0) {return;}
	unsigned int x = getScaler().getObjectWidth(beat);
	slider->move(x, 0);
}

#include <QCoreApplication>
void Editor::onTrackSelectionChanged(SequencerTrack* st) {

	// delete old sheet (if any)
	if (sheet)			{delete sheet; sheet = nullptr;}
	if (sheetHeader)	{delete sheetHeader; sheetHeader = nullptr;}

	// create new sheet?
	if (st == nullptr) {return;}

	// create new sheet
	sheet = new EditorSheet(*this, *st, this);
	sheetHeader = new EditorSheetHeader(*sheet);
	layout->addWidget(sheet);
	layoutHeader->addWidget(sheetHeader);
	sheetHeader->setVisible(true);

	// FIXME: dirty hack.. workaround?
	QCoreApplication::processEvents();

	slider->raise();

	resizeMe();

}

QWidget* Editor::getHeaderWidget() const {
	return header;
}

#include<vector>
std::vector<EditorNote> Editor::getNotes(SequencerTrack& st) const {

	// store all notes here
	std::vector<EditorNote> notes;

	for (MidiEvent* evt : *st.getEvents()) {

		if (evt->getType() == MidiEventType::NOTE_ON) {

			// note on? -> new entry
			notes.push_back(EditorNote( evt ));

		} else if (evt->getType() == MidiEventType::NOTE_OFF) {

			// lambda to find matching element
			auto lambda = [&evt] (const EditorNote& o) {
				return o.on->getData1() == evt->getData1() && o.off == nullptr;
			};

			// found?
			auto it = std::find_if( notes.begin(), notes.end(), lambda );
			if (it != notes.end()) {
				it->off = evt;
			}

		}

	}

	return notes;

}


TimeBase128 Editor::getSongLength() const {
	return ctx.getSequencer()->getSongLength() + 128*4*4;
}

void Editor::resizeMe() {

	if (sheet)			{emit sheet->onZoom();}
	if (sheetHeader)	{emit sheetHeader->updateGeometry();}

	layout->invalidate();
	layoutHeader->invalidate();

	QSize size = layout->sizeHint();
	this->resize(size.width(), size.height());

	// adjust slider's height
	slider->resize(0, size.height());

}

void Editor::zoomUp() {
	scaler.setBase(scaler.getBase()*2);
	resizeMe();
}

void Editor::zoomDown() {
	scaler.setBase(scaler.getBase()/2);
	resizeMe();
}

EditorScaler& Editor::getScaler() {
	return scaler;
}
