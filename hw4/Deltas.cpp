/*
 * Deltas.cpp
 *
 *  Created on: Jul 24, 2016
 *      Author: user
 */

#include "Deltas.h"

Deltas::Deltas(double x, double y, double yaw) : x(x), y(y), yaw(yaw) { }

double Deltas::getX() const {
	return x;
}

double Deltas::getY() const {
	return y;
}

double Deltas::getYaw() const {
	return yaw;
}

void Deltas::set(double x, double y, double yaw) {
	this->x = x;
	this->y = y;
	this->yaw = yaw;
}

std::ostream& operator<<(std::ostream& out, const Deltas& deltas) {
	return out << deltas.x << " " << deltas.y << " " << deltas.yaw;
}
