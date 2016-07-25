/*
 * LocalizationManager.h
 *
 *  Created on: Jun 7, 2016
 *      Author: colman
 */

#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include <vector>
#include "Particle.h"
#include "Position.h"
#include "Deltas.h"

class LocalizationManager {
private:
    vector<Particle> particles;
    const Map* map;
public:
    LocalizationManager();
    LocalizationManager(const Position& start, const Map* map);

	Position update(const LaserProxy& lp, const Deltas& deltas);

	 //getting the value of the average belief of particles
	 double getMidBelife();

	 void concatVector(vector<Particle>& lhs, vector<Particle>& rhs);

	 void printParticels(const char* mapFilePath);

};

#endif /* LOCALIZATIONMANAGER_H_ */
