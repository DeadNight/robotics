/*
 * Deltas.h
 *
 *  Created on: Jul 24, 2016
 *      Author: user
 */

#ifndef DELTAS_H_
#define DELTAS_H_

class Deltas {
	double x;
	double y;
	double yaw;
public:
	Deltas(double x, double y, double yaw);

	double getX() const;
	double getY() const;
	double getYaw() const;
};

#endif /* DELTAS_H_ */
