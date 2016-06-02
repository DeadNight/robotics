/*
 * Partical.h
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#ifndef PARTICAL_H_
#define PARTICAL_H_

#include "Position.h"
#include "Robot.h"
#include "MapSearchable.h"
#include <vector>
#include <math.h>

#define PI 3.14159265

class Partical {
	Position position;
	double belief;
public:

	Partical(Position p);

	double getBelief() const;

	void setBelief(double belief);

	const Position& getPosition() const;

	void setPosition(const Position& position);

	void update(LaserProxy *lp,double dx, double dy, double dyaw, MapSearchable map);

	void printPosition();

	Path linearPath(Location a, Location b);

	double probaByLazer(LaserProxy *lp, MapSearchable map);

	double probaByMove(double dx, double dy, double dyaw);
};


#endif /* PARTICAL_H_ */
