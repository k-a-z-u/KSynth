#include "SequencerTracksWidget.h"

#include "model/Context.h"
#include <QVBoxLayout>
#include "SequencerTrackItem.h"
#include "../controller/Controller.h"

SequencerTracksWidget::SequencerTracksWidget(Context& ctx, QWidget *parent) :
	QWidget(parent), ctx(ctx), lay(nullptr), selected(nullptr) {

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

//void SequencerTracksWidget::addListener(SequencerTracksListener* l) {
//	listeners.push_back(l);
//}

//void SequencerTracksWidget::removeListener(SequencerTracksListener* l) {
//	auto match = [l] (const SequencerTracksListener* other) {return other == l;};
//	listeners.erase( std::remove_if(listeners.begin(), listeners.end(), match), listeners.end() );
//}


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

SequencerTrackItem* SequencerTracksWidget::getSelectedItem() {
	return selected;
}

void SequencerTracksWidget::setSeleceted(SequencerTrackItem& item) {
	if (selected) {selected->setSelected(false);}
	selected = &item;
	selected->setSelected(true);
//	for (SequencerTracksListener* l : listeners) {l->selectedTrackChanged(&item.getTrack());}
	emit onCurrentItemChanged( &item.getTrack() );
}

#include <QSpacerItem>
void SequencerTracksWidget::refreshMe() {

	RemoveLayout(this);

	lay = new QVBoxLayout();
	lay->setMargin(0);
	lay->setSpacing(0);

	//qDeleteAll(findChildren<QWidget*>());
	for (SequencerTrack& st : *ctx.getSequencer()->getTracks()) {
		lay->addWidget(new SequencerTrackItem(*this, *ctx.getRack(), st));
	}
	lay->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding));

	// clear current selection
	selected = nullptr;

	setLayout(lay);

}


