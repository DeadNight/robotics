/*
 * Deltas.h
 *
 *  Created on: Jul 24, 2016
 *      Author: user
 */

#ifndef DELTAS_H_
#define DELTAS_H_

#include <iostream>

class Deltas {
	double x;
	double y;
	double yaw;
public:
	Deltas() : x(0), y(0), yaw(0) { }
	Deltas(double x, double y, double yaw);

	double getX() const;
	double getY() const;
	double getYaw() const;

	void set(double x, double y, double yaw);

	friend std::ostream& operator<<(std::ostream& out, const Deltas& deltas);
};

#endif /* DELTAS_H_ */
