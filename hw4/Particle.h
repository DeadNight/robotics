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

#define NORMAL 1.1
#define RANGE 1
#define ROTATION 20
#define THROW_THRESHOLD 0.7
#define SPAWN_THRESHOLD 0.8
#define SPAWN_THRESHOLD2 0.9

class Particle {
	const Map* map;
	Position position;
	double belief;
public:
	Particle() { }
	Particle(const Map* map, const Position& p, double belief);

	double getBelief() const;
	void setBelief(double belief);

	Position getPosition() const;
	void setPosition(const Position& position);

	void set(const Particle& p);

	void update(const LaserProxy& lp, const Deltas& deltas);

	double hitTest(double distance, double angle, double maxDistance);

	double helpFunc (double x);

	double probaByLazer(const LaserProxy& lp);

	double probaByMove(const Deltas& deltas);

	Position spawnPosition();

	vector<Particle>::iterator find(vector<Particle>& list, const Particle& p);

	vector<Particle> spawnParticles(int num);

	void save(const char* mapFilePath) const;
	void save(Image& image) const;

	inline bool operator==(const Particle& other) const {
		return position == other.position;
	}

	inline bool operator!=(const Particle& other) const { return !(*this == other); }
};

#endif /* PARTICLE_H_ */
