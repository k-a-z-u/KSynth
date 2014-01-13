#ifndef SNAPPER_H
#define SNAPPER_H

#include <vector>

/**
 * this provides snapping support to stick
 * adjusted controls to snapping values identified by a
 * point and a region around this point where the snapping
 * takes place.
 */


/** one snapping setting */
template <typename T> struct Snap {

	/** the center to snap to */
	T value;

	/** the region [snap-size;snap+size] where to apply the snapping */
	T size;

	/** perform snapping. will adjust the given value if within region */
	T getSnapped(T val) const {
		if (val < value-size) {return val;}
		if (val > value+size) {return val;}
		return value;
	}

	/** ctor */
	Snap(T value, T size) : value(value), size(size) {;}


};

/** convenience class for snapping */
template <typename T> class Snapper {

public:

	/** get snapped version of the given value */
	T getSnapped(T val) {
		for (const Snap<T>& s : snaps) {val = s.getSnapped(val);}
		return val;
	}

	/** add a new snapping entry */
	void add(T val, T size) {
		snaps.push_back(Snap<T>(val, size));
	}

private:

	/** all snapping entries */
	std::vector<Snap<T>> snaps;

};

#endif // SNAP_H
