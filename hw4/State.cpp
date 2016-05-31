/*
 * State.cpp
 *
 *  Created on: May 10, 2016
 *      Author: colman
 */

#include "State.h"

State::State(): baseCost(0), locationCost(0), heuristicCost(0), prevState(NULL) {
}

State::State(const Location& l): location(l), baseCost(0), locationCost(0), heuristicCost(0), prevState(NULL) {
}

State::State(const Location& l, double bc, double lc, double hc): location(l), baseCost(bc), locationCost(lc), heuristicCost(hc), prevState(NULL) {
}

State::State(const Location& l, double bc, double lc, double hc, const State& prevState): location(l), baseCost(bc), locationCost(lc), heuristicCost(hc), prevState(&prevState) {
}

double State::getBaseCost() const {
	return baseCost;
}

void State::setBaseCost(double c) {
	this->baseCost = c;
}

double State::getCost() const {
	return baseCost + locationCost + heuristicCost;
}

Location State::getLocation() const {
	return location;
}

void State::setLocation(Location l) {
	location = l;
}

const State* State::getPrevState() const {
	return prevState;
}

void State::setPrevState(const State& s) {
	prevState = &s;
}

double State::getLocationCost() const {
	return locationCost;
}

void State::setLocationCost(double lc) {
	locationCost = lc;
}
