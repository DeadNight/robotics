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
    Map map;
public:
    LocalizationManager();
    LocalizationManager(vector<Partical *> _particles,Map map);
    virtual ~LocalizationManager();

    const Map& getMap() const ;
	void setMap(const Map& map);

	const vector<Partical*>& getParticles() const;
	void setParticles(const vector<Partical*>& particles);

	 void update(LaserProxy *lp,double dx, double dy, double dyaw);

	 void resampleParticles(unsigned i);
	 Partical *getBestParticle();

	 //getting the value of the average belief of particles
	 double getMidBelife();

	void addVector (vector<Partical*>& addVector);

	 void printParticels(const char* mapFilePath, float mapResolution);

};

#endif /* LOCALIZATIONMANAGER_H_ */
