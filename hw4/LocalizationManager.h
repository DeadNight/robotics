/*
 * LocalizationManager.h
 *
 *  Created on: Jun 7, 2016
 *      Author: colman
 */

#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include "Partical.h"
#include <vector>
using namespace std;

class LocalizationManager {
private:
    vector<Partical *> _particles;
public:
	LocalizationManager();

	 void update(double deltaX, double deltaY, double deltaYaw);
	 void resampleParticles();
	 Partical *getBestParticle();
	 
	 virtual ~LocalizationManager();
};

#endif /* LOCALIZATIONMANAGER_H_ */
