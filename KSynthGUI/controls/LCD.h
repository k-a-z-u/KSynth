#ifndef LCD_H
#define LCD_H

#include <QWidget>
#include <QPen>

class LCD : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit LCD(QWidget *parent = 0);

	/** set the values to display */
	void setValues( const std::vector<float>& next);

	/** resize the LCD */
	void setSize(int w, int h);

	/** draw values centered around height/2 */
	void setCentered(bool centered);

	/** is the LCD in centered mode? */
	bool isCentered() const;

protected slots:

	/** repaint the LCD */
	void paintEvent(QPaintEvent* event) override;

private:

	void updateBG();

	/** the LCDs dotted background */
	QImage bg;

	std::vector<float> values;

	struct {
		QVector<qreal> dashes;
		QPen pen;
	} paint;

	bool centered;

};

#endif // LCD_H
