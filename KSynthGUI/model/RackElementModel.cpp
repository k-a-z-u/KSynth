
// DEPRECATED
// ADDED TO SequencerTrackItem... much easier

/*
#include "RackElementModel.h"
#include "../rack/RackElement.h"
#include "../rack/Rack.h"
#include <KSynth/src/misc/SoundBase.h>


RackElementModel::RackElementModel(Rack& rack) : rack(rack) {
	connect(&rack, SIGNAL(entryAdded(int)), this, SLOT(onInsert(int)));
	connect(&rack, SIGNAL(entryRemoved(int)), this, SLOT(onDelete(int)));
	connect(&rack, SIGNAL(entryChanged(int)), this, SLOT(onEdit(int)));
	devs = rack.getNoteDevices();
}

int RackElementModel::rowCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return int(devs.size()) + 1;
}

int RackElementModel::columnCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return 1;
}

void RackElementModel::onInsert(int idx) {
	emit beginResetModel();
	//emit beginInsertRows( QModelIndex(), idx+1, idx+1);										// + 1 for one dummy entry at beginning of the list
	devs = rack.getNoteDevices();
	//emit endInsertRows();
	emit endResetModel();
}

void RackElementModel::onDelete(int idx) {
	emit beginResetModel();
	//emit beginRemoveRows( QModelIndex(), idx+1, idx+1);								// + 1 for one dummy entry at beginning of the list
	devs = rack.getNoteDevices();
	//emit endRemoveRows();
	emit endResetModel();
}

void RackElementModel::onEdit(int idx) {
	devs = rack.getNoteDevices();
	emit dataChanged( index(idx+1, 0, QModelIndex()), index(idx+1, 0, QModelIndex()));		// + 1 for one dummy entry at beginning of the list
}

#include <iostream>
QModelIndex RackElementModel::index(int row, int column, const QModelIndex &parent) const {
	Q_UNUSED(parent);
	if (row <= 0)	{return createIndex( row, column, nullptr );}
	--row;
	if (row < (int) devs.size()) {
		return createIndex( row, column, devs.at(row) );
	}
	return createIndex( row, column, nullptr );
}

QModelIndex RackElementModel::parent(const QModelIndex &child) const {
	Q_UNUSED(child);
	return QModelIndex();
}


QVariant RackElementModel::data(const QModelIndex &index, int role) const {

	int row = index.row();

	switch (role) {
		case Qt::DisplayRole: {
				if (row <= 0) {return "----------------";}
				--row;
				return devs.at(row)->getUserName().c_str();
			}
		case Qt::UserRole: {
				if (row <= 0) {return qVariantFromValue( (void*) nullptr );}
				--row;
				return qVariantFromValue( (void*) devs.at( row ) );
			}
		default: {
				return "???";
			}
	}
}
*/
