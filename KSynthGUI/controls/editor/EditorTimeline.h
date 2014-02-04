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

	void mousePressEvent(QMouseEvent*) override;

private:

	Editor& editor;

	/** the last base after painting (used for caching) */
	unsigned int lastNW4;

signals:

public slots:

};

#endif // EDITORTIMELINE_H
