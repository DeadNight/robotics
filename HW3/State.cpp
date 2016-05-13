/*
 * State.cpp
 *
 *  Created on: May 10, 2016
 *      Author: colman
 */

#include "State.h"

State::State(Location l, unsigned char wd) {
	setLocation(l);
	setCost(wd);
	setPrevState(NULL);
	setWallDist(wd);
}

State::~State(){
	delete _prevState;
}

	double State::getCost() const{
		return cost;
}

	void State::setCost(double a){
		this->cost = a;
}

Location State::getLocation()const{
	return location;
}

void State::setLocation(Location l){
	this->location = l;
}

State* State::getPrevState()const{
	return _prevState;
}

void State::setPrevState(State* _l){
	this->_prevState = _l;
}

unsigned char State::getWallDist() const {
	return wallDist;
}

void State::setWallDist(unsigned char wallDist) {
	this->wallDist = wallDist;
}

bool operator<(const State& l, const State& r){
	return l.cost > r.cost;
}

bool State::eq(State s1, State s2){
	return (s1.getLocation().getX()==s2.getLocation().getX()) && (s1.getLocation().getY()==s2.getLocation().getY());
}

double State::UpdatedCost() //update the cost of the state
{
cost = wallDist + _prevState->getCost();
return cost;
}
