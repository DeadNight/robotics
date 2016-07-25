/*
 * Particle.h
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "Image.h"
#include "Color.h"
#include "Position.h"
#include "Robot.h"
#include "MapSearchable.h"

#define PI 3.14159265
#define NORMAL 1.2
#define RANGE 3

class Particle {
	Position position;
	double belief;
public:
	Particle() { }
	Particle(const Map& map, const Position& p, double belief);

	double getBelief() const;

	void setBelief(double belief);

	const Position& getPosition() const;

	void setPosition(const Position& position);

	void update(const LaserProxy& lp, const Deltas& deltas, const Map& map);

	//in order to print one particle
	void printParticle(const Map& map, const char* mapFilePath);
	//in order to save particle to image but not only one
	void printParticle(const Map& map, Image& image);

	Path linearPath(const Location& a, const Location& b);

	double helpFunc (double x);

	double probaByLazer(const LaserProxy& lp, const Map& map);

	double probaByMove(const Deltas& deltas);

	vector<double> random_numbers();

	vector<Particle>::iterator find(vector<Particle>& list, const Particle& p);

	vector<Particle> spawnParticles(int num, const Map& map);

	inline bool operator==(const Particle& other) const {
		return (position.getX() == other.getPosition().getX()) && (position.getY() == other.getPosition().getY()) &&(position.getYaw() == other.getPosition().getYaw());
	}

	inline bool operator!=(const Particle& other) const { return !(*this == other); }
};

#endif /* PARTICLE_H_ */
