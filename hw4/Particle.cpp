/*
 * Partical.cpp
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#include "Particle.h"

Particle::Particle(const Map* map, const Position& p, double belief) : map(map), belief(belief) {
	setPosition(p);
}


double Particle::getBelief() const {
	return belief;
}

void Particle::setBelief(double belief) {
	this->belief = belief;
}

Position Particle::getPosition() const {
	return map->getWorldPosition(position);
}

void Particle::setPosition(const Position& position) {
	this->position.set(map->getGridPosition(position));
}

void Particle::set(const Particle& p) {
	position.set(p.position);;
	belief = p.belief;
}

void Particle::update(const LaserProxy& lp, const Deltas& deltas) {
	position.set(
		position.getX() + deltas.getX()/map->getGridResolution(),
		position.getY() - deltas.getY()/map->getGridResolution(),
		position.getYaw() + deltas.getYaw()
	);

	if(deltas.getX() == 0 && deltas.getY() == 0 && deltas.getYaw() == 0)
		return;

	double newBelief = NORMAL * belief * probaByMove(deltas) * probaByLazer(lp);

	if(!isnan(newBelief))
		belief = newBelief > 1 ? 1 : newBelief;
}

double Particle::hitTest(double distance, double angle, double maxDistance) {
	double x, y, dx, dy, xe, ye, d;

	Location obs(
		position.getX() + distance*cos(angle),
		position.getY() - distance*sin(angle)
	);

	dx = obs.getX() - position.getX();
	dy = obs.getY() - position.getY();

	x = position.getX();
	y = position.getY();
	xe = obs.getX();
	ye = obs.getY();

	if(fabs(dy) <= fabs(dx)) {
		dy = dy / fabs(dx);
		dx = dx / fabs(dx);
	} else {
		dx = dx / fabs(dy);
		dx = dy / fabs(dy);
	}

	while((fabs(dx) == 1 && ((dx >= 0 && x < xe) || (dx < 0 && x > xe)))
			|| (fabs(dy) == 1 && ((dy >= 0 && y < ye) || (dy < 0 && y > ye)))) {
		x += dx;
		y += dy;

		d = sqrt(pow(position.getX() - x, 2) + pow(position.getY() - y, 2));
		if((*map)(round(x), round(y)) == 1) {
			return distance < maxDistance ? d : maxDistance;
		} else if(d >= distance + 3) {
			return distance < maxDistance ? d : INT_MAX;
		}
	}

	 return sqrt(pow(position.getX() - x, 2) + pow(position.getY() - y, 2));
}

double Particle::probaByLazer(const LaserProxy& lp) {
	double hits = 0;
	double dMax = 100 * lp.GetMaxRange() / map->getGridResolution();

	for(uint i = 0; i < lp.GetCount(); ++i) {
		double d = 100 * lp[i] / map->getGridResolution();
		double angle = position.getYaw() + lp.GetBearing(i);

		double hit = hitTest(d, angle, dMax);

		if(fabs(hit - d) < 1) {
			++hits;
		} else if(fabs(hit - d) < 2) {
			hits += 0.9;
		} else if(fabs(hit - d) < 3) {
			hits += 0.5;
		}
	}

	return hits / lp.GetCount();
}

double Particle::probaByMove(const Deltas& deltas) {
	double disntanceFactor, rotationFactor;

	double disntance = sqrt(pow(deltas.getX(), 2) + pow(deltas.getY(), 2));
	if(disntance > 100)
		disntanceFactor = 0.5;
	else
		disntanceFactor = 0.5 + 0.5*(100 - disntance)/100;

	double rotation = fabs(deltas.getYaw());
	if(rotation > 180)
		rotationFactor = 0.5;
	else
		rotationFactor = 0.5 + 0.5*(180 - rotation)/180;

	return disntanceFactor * rotationFactor;
 }

Position Particle::spawnPosition() {
	return Position(
			position.getX() + (rand() % (2*RANGE + 1)) - RANGE,
			position.getY() + (rand() % (2*RANGE + 1)) - RANGE,
			position.getYaw() + rand() % (2*ROTATION + 1) - ROTATION
	);
}

vector<Particle>::iterator Particle::find(vector<Particle>& list, const Particle& p) {
	for(vector<Particle>::iterator it = list.begin(); it != list.end(); ++it)
		if(p == *it)
			return it;
	return list.end();
}

vector <Particle> Particle::spawnParticles(int num) {
	vector<Particle> particles;

	srand(time(NULL));

	for(int i = 0; i < num; ++i) {
		Position pos = spawnPosition();

		if((*map)(pos.getLocation()) != 1) {
			Particle newParticle = Particle(map, map->getWorldPosition(pos), belief);

			if(particles.end() == find(particles, newParticle))
				particles.push_back(newParticle);
		}
	}

	return particles;
}

void Particle::save(const char* mapFilePath) const {
	Image image = map->toImage();
	double resolutionRatio = (double)map->getGridResolution() / map->getMapResolution();
	Position imagePosition = position * resolutionRatio;

	image.setAround(imagePosition.getX(), imagePosition.getY(), 4, Color::Magenta);
	image.save(mapFilePath);
}

void Particle::save(Image& image) const {
	double resolutionRatio = (double)map->getGridResolution() / map->getMapResolution();
	Position imagePosition = position * resolutionRatio;

	Color color = Color::Red;
	if(belief > SPAWN_THRESHOLD2) {
		color = Color::Green;
	} else if(belief > SPAWN_THRESHOLD) {
		color = Color::Yellow;
	}

	image.setAround(imagePosition.getX(), imagePosition.getY(), 2, color);
}
