#ifndef EDITORSLIDER_H
#define EDITORSLIDER_H

#include <QWidget>

class Context;
class Editor;

class EditorSlider : public QWidget  {
	Q_OBJECT

public:

	/** ctor */
	explicit EditorSlider(Editor& editor, Context& ctx, QWidget *parent = 0);

	/** dtor */
	~EditorSlider();

protected:

	void paintEvent(QPaintEvent *) override;


private:

	Editor& editor;

	Context& ctx;

signals:

public slots:

};

#endif // EDITORSLIDER_H
