/*
 * LocalizationManager.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: colman
 */

#include "LocalizationManager.h"

LocalizationManager::LocalizationManager() {}

LocalizationManager::LocalizationManager(vector<Partical *> _particles, Map map) {
	setParticles(_particles);
	setMap(map);
}

LocalizationManager::~LocalizationManager(){}

const vector<Partical*>& LocalizationManager::getParticles() const {
		return _particles;
	}

void LocalizationManager::setParticles(const vector<Partical*>& particles) {
	_particles = particles;
}

const Map& LocalizationManager::getMap() const {
		return map;
	}

void LocalizationManager::setMap(const Map& map) {
	this->map = map;
}

void LocalizationManager::update(LaserProxy *lp,double dx, double dy, double dyaw){
	unsigned size = _particles.size();
	for (unsigned i =0; i < size; i++){
		_particles[i]->update(lp,dx,dy,dyaw,&this->map);
		resampleParticles(i);
	 }
}

 void LocalizationManager::resampleParticles(unsigned i){
	 double middle = getMidBelife();
	 vector <Partical*> multiPart;
		 if(_particles[i]->getBelief()<=(middle/2)){
			_particles.erase (_particles.begin()+3);
		 }
		 else if((_particles[i]->getBelief()>=(middle)) && (_particles[i]->getBelief()<(middle*3/2) )){
				multiPart = _particles[i]->particleMulti(2,&this->map);
				addVector(multiPart);
		 }
		 else if((_particles[i]->getBelief()>=(middle*3/2))){
				multiPart = _particles[i]->particleMulti(4,&this->map);
				addVector(multiPart);
		 }
 }

 Partical* LocalizationManager::getBestParticle(){
	 double max =0;
	 Partical *temp;
	 for (unsigned i =0; i < _particles.size(); i++){
		 if(_particles[i]->getBelief()>max){
			 max = _particles[i]->getBelief();
			 temp = _particles[i];
		 }
	 }
	 return temp;
 }

 double LocalizationManager::getMidBelife(){
	 double max =0;
	 double min =1;
	 for (unsigned i =0; i < _particles.size(); i++){
		 if(_particles[i]->getBelief()>max){
			 max = _particles[i]->getBelief();
		 }
		 if(_particles[i]->getBelief()<min){
		 			 min = _particles[i]->getBelief();
		 }
	 }
	 double mid = (min+max)/2;
	 return mid;
 }

 void LocalizationManager::addVector (vector<Partical*>& addVector){
	 std::vector<Partical*>::iterator it = this->_particles.end();
	 this->_particles.insert(it,addVector.begin(),addVector.end());
 }

 void LocalizationManager::printParticels(const char* mapFilePath, float mapResolution){
	Image image = map.toImage(mapResolution);
	 for (unsigned i =0; i < _particles.size(); i++){
		 _particles[i]->printPartical(&this->map,&image,mapResolution);
	 }
	 image.save(mapFilePath);
 }
