#include "SequencerWidget.h"
#include "ui_SequencerWidget.h"

#include "model/Context.h"
#include "controls/SequencerTrackItem.h"
#include "rack/RackElement.h"
#include "controls/SequencerTracksWidget.h"

///** glue between the sequencer backend and the GUI */
//class SequencerTracksModel : public QAbstractItemModel, public SequencerTrackListener {

//public:

//	SequencerTracksModel(Context& ctx) : ctx(ctx) {
//		ctx.seq->addListener(this);
//	}
//	int rowCount(const QModelIndex &parent = QModelIndex()) const {
//		Q_UNUSED(parent);
//		return (int) ctx.seq->getTracks()->size();
//	}
//	int columnCount(const QModelIndex &parent = QModelIndex()) const {
//		Q_UNUSED(parent);
//		return 2;
//	}
//	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const {
//		Q_UNUSED(parent);
//		return createIndex( row, column, nullptr);
//	}
//	QModelIndex parent(const QModelIndex &child) const {
//		Q_UNUSED(child);
//		return QModelIndex();
//	}
//	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
//		Q_UNUSED(role);
//		unsigned int row = index.row();
//		switch(index.column()) {
//			case 0: return QVariant( (const char*) ctx.seq->getTracks()->at(row).getName().c_str());
//			case 1: return qVariantFromValue( (void*) ctx.seq->getTracks()->at(row).getDevice() );
//			default: return QVariant("ERR");
//		}
//	}
//	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {
//		(void) role;
//		unsigned int row = index.row();
//		switch (index.column()) {
//			case 0: ctx.seq->getTracks()->at(row).setName(value.toString().toStdString()); break;
//		}
//		return true;
//	}
//	Qt::ItemFlags flags(const QModelIndex &index) const {
//		(void) index;
//		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//	}

//	void onTracksChanged() override {
//		emit reset();
//	}

//private:

//	Context& ctx;

//};

//class RackElementModel : public QAbstractItemModel {
//public:
//	int rowCount(const QModelIndex &parent = QModelIndex()) const {
//		Q_UNUSED(parent);
//		return (int) RackElement::getElements().size() + 1;
//	}
//	int columnCount(const QModelIndex &parent = QModelIndex()) const {
//		Q_UNUSED(parent);
//		return 1;
//	}
//	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const {
//		Q_UNUSED(parent);
//		void* ptr = nullptr;
//		if (row == 0)	{ptr = nullptr;}
//		if (row > 0)	{ptr = RackElement::getElements().at(row - 1);}
//		return createIndex( row, column, ptr );
//	}
//	QModelIndex parent(const QModelIndex &child) const {
//		Q_UNUSED(child);
//		return QModelIndex();
//	}
//	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
//		Q_UNUSED(role);
//		if (index.row() == 0) {return "None";}
//		return "SDFSLDFK";//RackElement::getElements().at(index.row() - 1)->getDeviceType().c_str();
//	}
//};




//#include <QPainter>
//#include <QComboBox>
//#include <QTextEdit>
//#include <QStyledItemDelegate>
//class SequencerTracksDelegate : public QStyledItemDelegate {
//public:
//	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

//		painter->save();

//		QVariant var = index.model()->data(index, Qt::UserRole);
//		int varType = var.userType();
//		switch ( varType ) {
//			case QVariant::Bool:
//				painter->drawText(option.rect,"BOOL"); break;
//			case QVariant::String:
//				painter->drawText(option.rect, var.toString()); break;
//			case 128: {
//					painter->translate(option.rect.left(), option.rect.top());
//					QComboBox* editor = new QComboBox();
//					editor->setModel(new RackElementModel());
//					editor->setGeometry(option.rect);
//					editor->addItem("LOOOL");
//					editor->render(painter);
//					delete editor;
//					//painter->drawText(option.rect,"PTR"); break;
//					break;
//				}
//			default:
//				painter->drawText(option.rect,"OTHER"); break;
//		}

//		painter->restore();

//	}
//	virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
//		Q_UNUSED(option);
//		Q_UNUSED(index);
//		return QSize(10,10);
//	}
//	QWidget* createEditor(QWidget *parent,
//						  const QStyleOptionViewItem &option,
//						  const QModelIndex &index) const {

//		switch (index.column()) {
//			case 0: {
//					QTextEdit* txt = new QTextEdit(parent);
//					txt->setGeometry(option.rect);
//					return txt;
//				}
//			case 1:{
//					QComboBox* cmb = new QComboBox(parent);
//					cmb->setModel(new RackElementModel());
//					cmb->setGeometry(option.rect);
//					return cmb;
//				}
//			default: return nullptr;
//		}

//	}
//};

SequencerWidget::SequencerWidget(Context& ctx, QWidget *parent) :
	QDockWidget(parent), ui(new Ui::SequencerWidget), ctx(ctx) {

	ui->setupUi(this);


	SequencerTracksWidget* w = new SequencerTracksWidget(ctx, ui->scrollTracks);
	ui->scrollTracks->setWidget(w);


}


SequencerWidget::~SequencerWidget() {
	delete ui;
}
