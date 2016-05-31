/*
 * Partical.cpp
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#include "Partical.h"

Partical::Partical(Position p) {
	this->position=p;

}

Partical::~Partical() {

}

double Partical::getBelief() const {
	return belief;
}

void Partical::setBelief(double belief) {
	this->belief = belief;
}

const Position& Partical::getPosition() const {
	return position;
}

void Partical::setPosition(const Position& position) {
	this->position = position;
}

void update(LaserProxy *lp,double dx, double dy, double dyaw, Map map){

}

void printPosition(){

}

double probaByLazer(LaserProxy *lp, Map map){

}

double probaByMove(double dx, double dy, double dyaw){
	double disntance = sqrt(pow(dx,2)+pow(dy,2));
	if (disntance<0.2 && dyaw <0.035)
		return 1;
	else if ()
}
