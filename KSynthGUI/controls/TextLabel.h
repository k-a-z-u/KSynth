#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <QWidget>

class TextLabel : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit TextLabel(QWidget *parent = 0);

	/** dtor */
	virtual ~TextLabel();

	/** set the label's text */
	void setText(const std::string& text);

	/** get the label's text */
	const std::string& getText() const;


protected:

	void paintEvent (QPaintEvent* event) override;

	void mouseReleaseEvent (QMouseEvent* e) override;

	void keyPressEvent(QKeyEvent *e) override;

	void focusInEvent(QFocusEvent *e) override;

	void focusOutEvent(QFocusEvent *e) override;

private:

	std::string text;

	struct State {
		bool focused;
		State() : focused(false) {;}
	} state;

signals:

	/** the text value has changed */
	void valueChanged();

public slots:

};

#endif // TEXTLABEL_H
