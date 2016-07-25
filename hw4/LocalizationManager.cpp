/*
 * LocalizationManager.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: colman
 */

#include "LocalizationManager.h"

LocalizationManager::LocalizationManager() { }

LocalizationManager::LocalizationManager(const Position& start, const Map* map) : map(map) {
	Particle particle(map, start, 1);
	particles.push_back(particle);
}

Position LocalizationManager::update(const LaserProxy& lp, const Deltas& deltas) {
	const Particle* bestParticle;
	vector<Particle> newParticles;
	vector<Particle>::iterator it = particles.begin();

	double max = INT_MIN, min = INT_MAX;

	for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
		Particle* p = &*it;
		p->update(lp, deltas);

		if(p->getBelief() > max) {
			max = p->getBelief();
			bestParticle = p;
		}

		if(p->getBelief() < min) {
			min = p->getBelief();
		}
	}

	double middle = (min + max)/2;

	std::cout << "#particles: " << particles.size() << " middle: " << middle << std::endl;
	if(!particles.size())
		throw "no particles";

	while(it != particles.end()) {
		Particle& p = *it;

		if(p.getBelief() <= middle / 2) {
			std::cout << "*** throwing " << p.getPosition() << " (" << p.getBelief() << ")" << std::endl;
			it = particles.erase(it);
		} else {
			double multiplier = 1;
			if(p.getBelief() >= middle && p.getBelief() < middle * 3/2) {
				std::cout << "*** spawning 2 " << p.getPosition() << " (" << p.getBelief() << ")" << std::endl;
				multiplier = 2;
			} else if(p.getBelief() >= middle * 3/2) {
				std::cout << "*** spawning 4 " << p.getPosition() << " (" << p.getBelief() << ")" << std::endl;
				multiplier = 4;
			}

			if(multiplier > 1) {
				vector<Particle> spawnedParticles = p.spawnParticles(multiplier);
				concatVector(newParticles, spawnedParticles);
			}

			++it;
		}
	}

	concatVector(particles, newParticles);

	return bestParticle->getPosition();
}

void LocalizationManager::concatVector(vector<Particle>& lhs, vector<Particle>& rhs){
	lhs.insert(lhs.end(), rhs.begin(), rhs.end());
}

 void LocalizationManager::printParticels(const char* mapFilePath){
	Image image = map->toImage();
	 for (unsigned i =0; i < particles.size(); i++){
		 particles[i].printParticle(image);
	 }
	 image.save(mapFilePath);
 }
