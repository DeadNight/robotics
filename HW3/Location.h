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

#include <iostream.h>

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

    friend ostream& operator<<(ostream& out, const Location& location);
    friend istream& operator>>(istream& in, Location& location);
};

#endif /* LOCATION_H_ */
