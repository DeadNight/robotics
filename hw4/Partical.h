/*
 * Partical.h
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#ifndef PARTICAL_H_
#define PARTICAL_H_

#include "Image.h"
#include "Color.h"
#include "Position.h"
#include "Robot.h"
#include "MapSearchable.h"
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#define PI 3.14159265
#define NORMAL 1.2
#define RANGE 3

class Partical {
	Position position;
	double belief;
public:

	Partical(Map *map,Position p, double belief);

	double getBelief() const;

	void setBelief(double belief);

	const Position& getPosition() const;

	void setPosition(const Position& position);

	void update(LaserProxy *lp,double dx, double dy, double dyaw, Map *map);

	void printPartical(Map *map,const char* mapFilePath);//in order to print one particle
	void printPartical(Map *_map,Image *image);//in order to save particle to image but not only one

	Path linearPath(Location a, Location b);

	double helpFunc (double x);

	double probaByLazer(LaserProxy *lp, Map *map);

	double probaByMove(double dx, double dy, double dyaw);

	vector<double> random_numbers();

	vector<Partical*>::iterator find(vector<Partical*>& list, const Partical& p);

	vector <Partical*> particleMulti(int num, Map *map);

	inline bool operator==(const Partical& other) const {
			return (position.getX() == other.getPosition().getX()) && (position.getY() == other.getPosition().getY()) &&(position.getYaw() == other.getPosition().getYaw());
		}
	inline bool operator!=(const Partical& other) const { return !(*this == other); }
};


#endif /* PARTICAL_H_ */
