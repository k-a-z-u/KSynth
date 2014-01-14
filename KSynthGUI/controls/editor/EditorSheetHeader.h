#ifndef EDITORSHEETHEADER_H
#define EDITORSHEETHEADER_H

#include <QWidget>
class EditorSheet;

class EditorSheetHeader : public QWidget {
	Q_OBJECT

public:

	EditorSheetHeader(EditorSheet& sheet, QWidget* parent = 0);

	QSize sizeHint() const override;

	void paintEvent(QPaintEvent*);


private:

	EditorSheet& sheet;

};
#endif // EDITORSHEETHEADER_H
