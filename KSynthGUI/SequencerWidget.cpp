#include "SequencerWidget.h"
#include "ui_SequencerWidget.h"

#include "model/Context.h"
#include "controls/SequencerTrackItem.h"
#include "rack/RackElement.h"
#include "controls/SequencerTracksWidget.h"
#include "controls/editor/Editor.h"
#include "controls/scroll/AdvScrollArea.h"
#include "controls/editor/EditorTimeline.h"
#include "controller/Controller.h"
#include "misc/Helper.h"

SequencerWidget::SequencerWidget(Context& ctx, QWidget *parent) :
	QDockWidget(parent), ui(new Ui::SequencerWidget), ctx(ctx) {

	ui->setupUi(this);

	// track selection widget
	tracks = new SequencerTracksWidget(ctx, ui->scrollTracks);
	ui->scrollTracks->setWidget(tracks);

	// timeline stuff
	AdvScrollArea* scroll = new AdvScrollArea();
	scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->editor->addWidget(scroll);

	// editor within scroll-area
	editor = new Editor(ctx, *tracks, *scroll);
	scroll->setWidget(editor);
	scroll->setHeaderH(new EditorTimeline(*editor), 28);
	scroll->setHeaderV(editor->getHeaderWidget(), 32);



	// signals
	connect(ui->btnZoomDown, SIGNAL(clicked()), editor, SLOT(zoomDown()));
	connect(ui->btnZoomUp, SIGNAL(clicked()), editor, SLOT(zoomUp()));

	connect(ui->btnTrackAdd, SIGNAL(clicked()), this, SLOT(addTrack()));
	connect(ui->btnTrackDelete, SIGNAL(clicked()), this, SLOT(deleteTrack()));

	ui->chkCursor->setProperty("MODE", int(EditorMode::CURSOR));
	connect(ui->chkCursor, SIGNAL(clicked()), this, SLOT(changeMode()));
	ui->chkDraw->setProperty("MODE", int(EditorMode::DRAW));
	connect(ui->chkDraw, SIGNAL(clicked()), this, SLOT(changeMode()));
	connect(ui->chkSnap, SIGNAL(clicked()), this, SLOT(changeSnap()));

	// snapping modes
	ui->cmbSnapInterval->addItem("1th");
	ui->cmbSnapInterval->addItem("2th");
	ui->cmbSnapInterval->addItem("4th");
	ui->cmbSnapInterval->addItem("8th");
	ui->cmbSnapInterval->addItem("16th");
	ui->cmbSnapInterval->addItem("32th");
	ui->cmbSnapInterval->addItem("64th");
	ui->cmbSnapInterval->addItem("128th");
	//ui->cmbSnapInterval->setEnabled(false);
	ui->cmbSnapInterval->setCurrentIndex(2);
	connect(ui->cmbSnapInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSnapInterval()));

}

void SequencerWidget::addTrack() {
	ctx.getController()->addNewTrack();
}

#include <QMessageBox>
void SequencerWidget::deleteTrack() {
	SequencerTrackItem* sel = tracks->getSelectedItem();
	if (!sel) {QMessageBox::information(this, "delete track", "no track selected"); return;}
	ctx.getSequencer()->removeTrack(sel->getTrack());
}

void SequencerWidget::changeMode() {
	QObject* obj = sender();
	EditorMode mode = (EditorMode) obj->property("MODE").toInt();
	editor->setMode(mode);
}

void SequencerWidget::changeSnap() {
	editor->getScaler().snap.enabled = ui->chkSnap->isChecked();
	//ui->cmbSnapInterval->setEnabled(ui->chkSnap->isChecked());
}

void SequencerWidget::changeSnapInterval() {
	unsigned int idx = ui->cmbSnapInterval->currentIndex();
	unsigned int snap = (unsigned int) std::pow(2, idx);
	editor->getScaler().snap.base = snap;
}

SequencerWidget::~SequencerWidget() {
	delete ui;
}
