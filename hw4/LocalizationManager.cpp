/*
 * LocalizationManager.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: colman
 */

#include "LocalizationManager.h"

LocalizationManager::LocalizationManager() { }

LocalizationManager::LocalizationManager(const Position& start, Map map) : map(map) {
	Particle particle(map, start, 1);
	particles.push_back(particle);
}

const Map& LocalizationManager::getMap() const {
	return map;
}

void LocalizationManager::setMap(const Map& map) {
	this->map = map;
}

const Position& LocalizationManager::update(const LaserProxy& lp, const Deltas& deltas) {
	double middle = getMidBelife();
	double max = 0;
	 const Particle* bestParticle;
	vector<Particle> newParticles;
	vector<Particle>::iterator it = particles.begin();

	while(it != particles.end()) {
		Particle& p = *it;
		p.update(lp, deltas, map);

		if(p.getBelief() <= middle / 2) {
			it = particles.erase(it);
		} else {
			if(p.getBelief() > max){
				 max = p.getBelief();
				 bestParticle = &p;
			 }

			double multiplier = 1;
			if(p.getBelief() >= middle && p.getBelief() < middle * 3/2) {
				multiplier = 2;
			} else if(p.getBelief() >= middle * 3/2) {
				multiplier = 4;
			}

			if(multiplier > 1) {
				vector<Particle> spawnedParticles = p.spawnParticles(multiplier, map);
				concatVector(newParticles, spawnedParticles);
			}

			++it;
		}
	}

	concatVector(particles, newParticles);

	return bestParticle->getPosition();
}

double LocalizationManager::getMidBelife() {
	double max = 0, min = 1;

	for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
		Particle& p = *it;
		if(p.getBelief() > max)
			max = p.getBelief();

		if(p.getBelief() < min)
			min = p.getBelief();
	}

	return (min + max)/2;
}

void LocalizationManager::concatVector(vector<Particle>& lhs, vector<Particle>& rhs){
	lhs.insert(lhs.end(), rhs.begin(), rhs.end());
}

 void LocalizationManager::printParticels(const char* mapFilePath){
	Image image = map.toImage();
	 for (unsigned i =0; i < particles.size(); i++){
		 particles[i].printParticle(map, image);
	 }
	 image.save(mapFilePath);
 }
