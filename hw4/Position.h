/*
 * Position.h
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <iostream>
#include "Location.h"

class Position {
	Location location;
	double yaw;
public:
	Position() { }
	Position(Location location, double yaw);
	Position(double x, double y, double yaw);

	Location getLocation() const;
    void setLocation(Location location);
    void setLocation(double x, double y);

    double getX() const;
    double getY() const;

    double getYaw() const;
    void setYaw(double yaw);

    void set(double x, double y, double yaw);

    inline Position& operator+=(const double scalar) {
		location += scalar;
		return *this;
	}
	friend Position operator+(Position position, const double scalar) {
		position += scalar;
		return position;
	}

	inline Position& operator*=(const double scalar) {
		location *= scalar;
		return *this;
	}
	friend Position operator*(Position position, const double scalar) {
		position *= scalar;
		return position;
	}

	inline Position& operator/=(const double scalar) {
		location /= scalar;
		return *this;
	}
	friend Position operator/(Position position, const double scalar) {
		position /= scalar;
		return position;
	}

    friend std::ostream& operator<<(std::ostream& out, const Position& position);
    friend std::istream& operator>>(std::istream& in, Position& position);
};

#endif /* POSITION_H_ */
