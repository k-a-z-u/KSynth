#ifndef SEQUENCER1SELECTOR_H
#define SEQUENCER1SELECTOR_H

#include <QWidget>
class Sequencer1;

class Sequencer1Selector : public QWidget {
	Q_OBJECT

public:

	explicit Sequencer1Selector(Sequencer1& seq, QWidget *parent = 0);


protected:

	void paintEvent (QPaintEvent* event) override;

	void mouseReleaseEvent (QMouseEvent* e) override;
	void mousePressEvent (QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;

	//void enterEvent(QEvent* e) override;
	void leaveEvent(QEvent* e) override;

private:

	void onMouse(int x, int y);

	Sequencer1& seq;

	struct Mouse {
		bool down;
		int curNoteRow;
		Mouse() : down(false), curNoteRow(-1) {;}
	} mouse;

signals:

public slots:


};

#endif // SEQUENCER1SELECTOR_H
