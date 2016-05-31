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
#include <math.h>

class Partical {
	Position position;
	double belief;
public:

	Partical(Position p);
	virtual ~Partical();

	double getBelief() const;

	void setBelief(double belief);

	const Position& getPosition() const;

	void setPosition(const Position& position);

	void update(LaserProxy *lp,double dx, double dy, double dyaw, Map map);

	void printPosition();

	double probaByLazer(LaserProxy *lp, Map map);

	double probaByMove(double dx, double dy, double dyaw);
};


#endif /* PARTICAL_H_ */
