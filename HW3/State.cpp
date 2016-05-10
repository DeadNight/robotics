/*
 * State.cpp
 *
 *  Created on: May 10, 2016
 *      Author: colman
 */

#include "State.h"

State::State(Location l) {
	setX(l.getX());
	setY(l.getY());
	setCost(1);
	MapSearchable ms;
	wallDist(ms(l.getX(),l.getY()));
}

State::State(const State& s) {
	setX(s.getX());
	setY(s.getY());
	setCost(1+s.getCost());
	wallDist(MapSearchable(s.getX(),s.getY()));
}

	int State::getCost(){
		return cost;
}

	void State::setCost(unsigned a){
		this->cost = a;
}

	int State::getWallDist(){
		return wallDist;
}

	void State::setWallDist(unsigned a){
		this->wallDist = a;
}

