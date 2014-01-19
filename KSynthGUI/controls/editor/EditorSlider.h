#ifndef EDITORSLIDER_H
#define EDITORSLIDER_H

#include "Grabable.h"

class Context;
class Editor;

class EditorSlider : public Grabable {
	Q_OBJECT

public:

	/** ctor */
	explicit EditorSlider(Editor& editor, Context& ctx, QWidget *parent = 0);

	/** dtor */
	~EditorSlider();

	void move(int x, int y);

protected:

	void paintEvent(QPaintEvent *) override;

	void onGrab(int x, int y, int w, int h) override;
	void onGrabDone(int x, int y, int w, int h) override;

private:

	Editor& editor;

	Context& ctx;

signals:

public slots:

};

#endif // EDITORSLIDER_H
