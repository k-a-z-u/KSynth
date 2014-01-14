#include "SequencerWidget.h"
#include "ui_SequencerWidget.h"

#include "model/Context.h"
#include "controls/SequencerTrackItem.h"
#include "rack/RackElement.h"
#include "controls/SequencerTracksWidget.h"
#include "controls/editor/Editor.h"
#include "controls/scroll/AdvScrollArea.h"

#include "controls/editor/EditorTimeline.h"
SequencerWidget::SequencerWidget(Context& ctx, QWidget *parent) :
	QDockWidget(parent), ui(new Ui::SequencerWidget), ctx(ctx) {

	ui->setupUi(this);

	// track selection widget
	SequencerTracksWidget* w = new SequencerTracksWidget(ctx, ui->scrollTracks);
	ui->scrollTracks->setWidget(w);

	// timeline stuff
	AdvScrollArea* scroll = new AdvScrollArea();
	scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->tracks->addWidget(scroll);

	// editor within scroll-area
	editor = new Editor(ctx, *w);
	scroll->setWidget(editor);
	scroll->setHeaderH(new EditorTimeline(*editor), 28);
	scroll->setHeaderV(editor->getHeaderWidget(), 32);

//	scr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//	scr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//	scr->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//	scr->setWidget(editor);

	//ui->scrollNotes->setWidget(editor);

	//setMinimumWidth(800);

	connect(ui->btnZoomDown, SIGNAL(clicked()), editor, SLOT(zoomDown()));
	connect(ui->btnZoomUp, SIGNAL(clicked()), editor, SLOT(zoomUp()));

}


SequencerWidget::~SequencerWidget() {
	delete ui;
}
