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
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265

class Partical {
	Position position;
	double belief;
public:

	Partical(Position p, double belief);

	double getBelief() const;

	void setBelief(double belief);

	const Position& getPosition() const;

	void setPosition(const Position& position);

	void update(LaserProxy *lp,double dx, double dy, double dyaw, MapSearchable map);

	void printPartical();

	Path linearPath(Location a, Location b);

	double probaByLazer(LaserProxy *lp, MapSearchable map);

	double probaByMove(double dx, double dy, double dyaw);

	vector<double> random_numbers();

	vector<Partical>::iterator find(vector<Partical>& list, const Partical& p);

	vector <Partical> particleMulti(Partical p, int num, MapSearchable map);

	inline bool operator==(const Partical& other) const {
			return (position.getX() == other.getPosition().getX()) && (position.getY() == other.getPosition().getY()) &&(position.getYaw() == other.getPosition().getYaw());
		}
	inline bool operator!=(const Partical& other) const { return !(*this == other); }
};


#endif /* PARTICAL_H_ */
