#ifndef SEQUENCERTRACKITEM_H
#define SEQUENCERTRACKITEM_H

#include <QWidget>

#include <KSynth/Sequencer.h>

class QLineEdit;
class QLabel;
class QComboBox;
class QCheckBox;
class Context;
class Rack;
class RackElementModel;


class SequencerTrackItem : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit SequencerTrackItem(Rack& ctx, SequencerTrack& st, QWidget *parent = 0);

	/** dtor */
	~SequencerTrackItem();

protected:

	void paintEvent (QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;

private:


	Rack& rack;

	/** the track's name */
	QLineEdit* txtName;

	/** the attached device */
	QComboBox* cmbDevice;

	/** muting */
	QCheckBox* chkMute;

	/** the underlying track */
	SequencerTrack& st;

//	/** the used model */
//	RackElementModel* cmbMdl;

private slots:

	/** text edited -> change track's name */
	void onChangeTrackName();

	/** combo-box changed -> change track's device */
	void onChangeTargetDevice();

	/** update the list of available entries within the combo box */
	void updateCombo();

signals:

public slots:

};

#endif // SEQUENCERTRACK_H
