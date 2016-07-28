/*
 * LocalizationManager.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: colman
 */

#include "LocalizationManager.h"

LocalizationManager::LocalizationManager(const Position& start, const Map* map) : map(map) {
	Particle particle(map, start, 1);
	particles.push_back(particle);
}

Position LocalizationManager::update(const LaserProxy& lp, const Deltas& deltas) {
	const Particle* bestParticle;
	vector<Particle> newParticles;
	vector<Particle>::iterator it = particles.begin();
	double max = INT_MIN;

	for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
		it->update(lp, deltas);

		if(it->getBelief() > max) {
			max = it->getBelief();
			bestParticle = &*it;
		}
	}

	//std::cout << "#particles: " << particles.size() << " max: " << max << std::endl;
	if(!particles.size())
		throw "no particles";

	while(it != particles.end()) {
		Particle& p = *it;
		int spawn = 0;

		if(p.getBelief() == max) {
			p.setBelief(1);
		}

		if(p.getBelief() <= THROW_THRESHOLD) {
			//std::cout << "*** throwing " << p.getPosition() << " (" << p.getBelief() << ")" << std::endl;
			it = particles.erase(it);
		} else {
			if(p.getBelief() >= SPAWN_THRESHOLD2) {
				spawn = 3;
			} else if(p.getBelief() >= SPAWN_THRESHOLD) {
				spawn = 1;
			}

			++it;
		}

		if(spawn > 0) {
			//std::cout << "*** spawning " << spawn << " " << p.getPosition() << " (" << p.getBelief() << ")" << std::endl;
			vector<Particle> spawnedParticles = p.spawnParticles(spawn);
			concatVector(newParticles, spawnedParticles);
		}
	}

	concatVector(particles, newParticles);

	return bestParticle->getPosition();
}

void LocalizationManager::concatVector(vector<Particle>& lhs, vector<Particle>& rhs){
	lhs.insert(lhs.end(), rhs.begin(), rhs.end());
}

const Particle& LocalizationManager::getBestParticle() const {
	const Particle* bestParticle;
	vector<Particle>::const_iterator it = particles.begin();
	double max = INT_MIN;

	for(vector<Particle>::const_iterator it = particles.begin(); it != particles.end(); ++it) {
		if(it->getBelief() > max) {
			max = it->getBelief();
			bestParticle = &*it;
		}
	}

	return *bestParticle;
}

 void LocalizationManager::save(const char* mapFilePath){
	Image image = map->toImage();
	 for (unsigned i = 0; i < particles.size(); i++){
		 particles[i].save(image);
	 }
	 image.save(mapFilePath);
 }
