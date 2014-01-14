#ifndef EDITORTIMELINE_H
#define EDITORTIMELINE_H

#include <QWidget>
#include "Editor.h"

class EditorTimeline : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit EditorTimeline(Editor& editor, QWidget *parent = 0);

protected:

	void paintEvent(QPaintEvent *) override;

private:

	Editor& editor;

signals:

public slots:

};

#endif // EDITORTIMELINE_H
