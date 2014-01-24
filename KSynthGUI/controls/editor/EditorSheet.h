#ifndef EDITORSHEET_H
#define EDITORSHEET_H

#include <QWidget>
class Editor;
class SequencerTrack;
class EditorSheetNote;
#include "EditorSheetDrawNote.h"

class EditorSheet : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit EditorSheet(Editor& editor, SequencerTrack& track, QWidget *parent = 0);

	/** get the sheet's parent */
	Editor& getEditor();

	/** get the underlying track (from the backend) containing all midi events */
	SequencerTrack& getTrack();


	QSize sizeHint() const override;

protected:

	void paintEvent(QPaintEvent*) override;

	void keyPressEvent(QKeyEvent*) override;

	void focusInEvent(QFocusEvent*) override;

	void focusOutEvent(QFocusEvent*) override;

	void resizeEvent(QResizeEvent*) override;


	friend class EditorSheetNote;

	/** select the given note */
	void setSelected(EditorSheetNote* note);

	/** unselect all notes */
	void setUnselected();


private:

	friend class EditorSheetDrawNote;

	/** update notes: sizes/positions */
	void updateNotes();


	/** parent */
	Editor& editor;

	/** the track to display the notes for */
	SequencerTrack& track;

	/** the current selection (if any) is stored here */
	std::vector<EditorSheetNote*> selection;

	/** this class provides functionality for adding new notes to the sheet */
	EditorSheetDrawNote drawer;


	/** the last base after painting (used for caching) */
	bool lastBase;

signals:

public slots:

	/** zoom changed */
	void onZoom();

};

#endif // EDITORSHEET_H
