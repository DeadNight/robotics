/*
 * Location.cpp
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#include "Location.h"

Location::Location(double x, double y) {
	setX(x);
	setY(y);
}

double Location::getX() const {
	return x;
}

void Location::setX(double x) {
	this->x = x;
}

double Location::getY() const {
	return y;
}

void Location::setY(double y) {
	this->y = y;
}

std::ostream& operator<<(std::ostream& out, const Location& location) {
	return out << location.x << " " << location.y;
}

std::istream& operator>>(std::istream& in, Location& location) {
	return in >> location.x >> location.y;
}
