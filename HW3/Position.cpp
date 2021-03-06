/*
 * Position.cpp
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#include "Position.h"

Position::Position(double x, double y, double yaw) {
	setLocation(Location(x, y));
	setYaw(yaw);
}

Position::Position(Location location, double yaw) {
	setLocation(location);
	setYaw(yaw);
}

Location Position::getLocation() const {
	return location;
}

void Position::setLocation(Location location) {
	this->location = location;
}

double Position::getX() const {
	return location.getX();
}
double Position::getY() const {
	return location.getY();
}

double Position::getYaw() const {
	return yaw;
}

void Position::setYaw(double yaw) {
	this->yaw = yaw;
}

std::ostream& operator<<(std::ostream& out, const Position& position) {
	return out << position.location << " " << position.yaw;
}

std::istream& operator>>(std::istream& in, Position& position) {
	return in >> position.location >> position.yaw;
}
