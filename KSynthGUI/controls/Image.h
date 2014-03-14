#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QImage>

class Image : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit Image(QWidget *parent = 0);

	/** set the image to display */
	void setImage(const QImage& img);

protected:

	void paintEvent(QPaintEvent*) override;

private:

	/** the image to show */
	QImage img;

signals:

public slots:

};

#endif // IMAGE_H
