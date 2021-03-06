/*
 * Location.h
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#ifndef LOCATION_H_
#define LOCATION_H_

#include <iostream>
#include <libplayerc++/playerc++.h>

class Location {
	double x;
	double y;
public:
	Location() { }
	Location(double x, double y);

    double getX() const;
    void setX(double x);

    double getY() const;
    void setY(double y);

    double angleTo(Location target) const;

    friend std::ostream& operator<<(std::ostream& out, const Location& location);
    friend std::istream& operator>>(std::istream& in, Location& location);
};

#endif /* LOCATION_H_ */
