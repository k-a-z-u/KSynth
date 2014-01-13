#ifndef LED1_H
#define LED1_H

#include <QWidget>

class LED1 : public QWidget {
	Q_OBJECT

public:
	explicit LED1(QWidget *parent = 0);

	/** change the status of the LED */
	void setValue(bool on);

protected:
	void paintEvent (QPaintEvent* event) override;

private:
	bool on;

signals:

public slots:

};

#endif // LED1_H
