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
