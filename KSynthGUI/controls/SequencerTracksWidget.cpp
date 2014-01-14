#include "SequencerTracksWidget.h"

#include "model/Context.h"
#include <QVBoxLayout>
#include "SequencerTrackItem.h"

SequencerTracksWidget::SequencerTracksWidget(Context& ctx, QWidget *parent) :
	QWidget(parent), ctx(ctx), lay(nullptr) {

	ctx.getSequencer()->addTrackListener(this);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//setMinimumSize(200, 400);
	setMaximumHeight(999999);
	setMaximumWidth(192);

}

SequencerTracksWidget::~SequencerTracksWidget() {

	// unreigster
	ctx.getSequencer()->removeTrackListener(this);

}


void SequencerTracksWidget::onTracksChanged() {
	emit refreshMe();
}


void RemoveLayout (QWidget* widget) {
	QObjectList c = widget->children();
	for (int i = 0; i < c.size(); ++i) {
		delete c.at(i);
	}
	delete widget->layout();
}

#include <QSpacerItem>
void SequencerTracksWidget::refreshMe() {

	//	if (lay) {
	//		while (lay->children().size()) {
	//			//delete lay->children().at(0);
	//			//lay->children().removeFirst();
	//			lay->removeWidget( (QWidget*) lay->children().at(0));
	//		}
	//		delete lay;
	//	}
	RemoveLayout(this);


	lay = new QVBoxLayout();
	lay->setMargin(1);
	lay->setSpacing(0);

	//qDeleteAll(findChildren<QWidget*>());
	for (SequencerTrack& st : *ctx.getSequencer()->getTracks()) {
		lay->addWidget(new SequencerTrackItem(*ctx.getRack(), st));
	}
	lay->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding));


	setLayout(lay);

}


