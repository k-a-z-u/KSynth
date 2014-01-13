#ifndef PINCONNECTOR_H
#define PINCONNECTOR_H

#include <QWidget>
#include <QDialog>
class QPushButton;
class RackElement;
class Context;
class SoundBase;
class QLabel;
class QComboBox;

#include <KSynth/misc/SoundBase.h>
#include <KSynth/SoundBaseBinder.h>

class PinConnector : public QWidget {
	Q_OBJECT

public:

	/** ctor */
	explicit PinConnector(Context& ctx, RackElement* re, QWidget* parent = 0);

	/** dtor */
	~PinConnector();


protected:

	void resizeEvent(QResizeEvent* event) override;

	void paintEvent (QPaintEvent* event) override;

	void mouseReleaseEvent (QMouseEvent* e) override;


private:

	Context& ctx;

	//QPushButton* btn;

	RackElement* re;

private slots:

	void startConnect();

	//void onConnect();

signals:

public slots:

};




enum class PinConEntryEdit {
	ATTACH_INPUT,
	ATTACH_OUTPUT
};


/** on entry to connect a source or a destination to one pin */
class PinConEntry : public QWidget {
	Q_OBJECT

public:
	PinConEntry(Context& ctx, BindingEndpoint src, PinConEntryEdit dir, QWidget* parent = 0);

private:
	void fetchTargets();

private slots:
	void onDstSel();
	void onDstPortSel();

private:
	Context& ctx;
	BindingEndpoint src;
	PinConEntryEdit dir;
	BindingEndpoint curEndpoint;
	QLabel* lblName;
	QComboBox* cmbDst;
	QComboBox* cmbDstPort;

};

/** the window to connect all inputs/outputs of one RackElement */
class PinConWindow : public QDialog {
	Q_OBJECT

public:
	PinConWindow(Context& ctx, RackElement* re, QWidget* parent = 0);
	~PinConWindow();

private:

};



#endif // PINCONNECTOR_H
