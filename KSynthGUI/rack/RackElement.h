#ifndef RACKELEMENT_H
#define RACKELEMENT_H

#include <QWidget>
class Context;
class TextLabel;

#include <KSynth/misc/DeviceDetails.h>

/**
 * @brief base class for every element to add to the rack.
 * each element will register itself to an update-thread called
 * peridodically to update all knobs, sliders, etc with the underlying
 * audio-generator classes.
 */
class RackElement : public QWidget, virtual public DeviceDetails {
	Q_OBJECT



public:

	/** ctor */
	explicit RackElement(Context& ctx, QWidget* parent = 0);

	/** dtor */
	virtual ~RackElement();

	void setUserName(const std::string& name) override;

	/**
	 * called when this element is attached to a rack.
	 * the element my register itself at various controls.
	 */
	virtual void onRackAttach() {;}

	/**
	 * the element will soon be detached from the rack and must take
	 * appropriate actions to de-register itself!
	 */
	virtual void onRackDetach() {;}

protected:

	/**
	 * @brief the RackElement uses this function to define its size
	 * @param width the RackElement's width
	 * @param height the RackElement's height
	 */
	void setSize(unsigned int width, unsigned int height);

	/** the context this element works with */
	Context& ctx;


	/** label to set the device's user name */
	TextLabel* label;

private:



signals:

	/** the user-name changed */
	void userNameChanged();


public slots:

	/** delete this element */
	void deleteMe();


private slots:


	/**
	 * @brief each RackElement must implement a refresh method to
	 * fetch all (volatile) settings from their underlying device
	 * and update all GUI-elements like knobs, sliders and VU-meters.
	 */
	virtual void refresh() = 0;


	/** show the context menu */
	void showContextMenu(const QPoint& pt);


	/** set the user-name from label and inform listeners */
	void updateUserName();

};

#endif // RACKELEMENT_H
