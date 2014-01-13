#ifndef RACKELEMENTMODEL_H
#define RACKELEMENTMODEL_H

// DEPRECATED
// ADDED TO SequencerTrackItem... much easier

//#include <KSynth/src/notes/NoteDevice.h>

//#include <QAbstractItemModel>
//Q_DECLARE_METATYPE(NoteDevice*);
//class Rack;

///**
// * wrap a QAbstractItemModel around all elements of the Rack.
// * when the contents of the rack change, this model will also change.
// */
//class RackElementModel : public QAbstractItemModel {
//	Q_OBJECT

//public:

//	/** ctor */
//	RackElementModel(Rack& rack);

//	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

//	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

//	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

//	QModelIndex parent(const QModelIndex &child) const override;

//	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

//private slots:

//	void onInsert(int idx);
//	void onDelete(int idx);
//	void onEdit(int idx);

//private:

//	Rack& rack;

//	std::vector<NoteDevice*> devs;

//};

#endif // RACKELEMENTMODEL_H
