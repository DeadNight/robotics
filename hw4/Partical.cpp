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
	this->position= position;
}

void Partical::update(LaserProxy *lp,double dx, double dy, double dyaw, MapSearchable map){
	this->position.setLocation(Location(this->position.getX()+dx,this->position.getY()+dy));
	this->position.setYaw(this->position.getYaw()+dyaw);
	this->belief=0.1*this->belief*probaByMove(dx,dy,dyaw)*probaByLazer(lp,map);
}

void Partical::printPosition(){

}

Path Partical::linearPath(Location from, Location to){
	unsigned top, bottom;
	unsigned fromY = from.getY();
	unsigned toY = to.getY();
	unsigned fromX = from.getX();
	unsigned toX = to.getX();
	std::vector<Location> temp;

		if(fromY < toY) {
			bottom = fromY; top = toY;
		} else {
			bottom = toY; top = fromY;
		}

		if(toX == fromX) {
			unsigned x = toX;
			for(unsigned y = bottom; y <= top; ++y)
				temp.push_back(Location(x,y));
		} else {
			unsigned left, right;

			double m = ((double)toY - fromY)/((double)toX - fromX);
			double n = toY - m*toX;

			if(fromX < toX) {
				left = fromX; right = toX;
			} else {
				left = toX; right = fromX;
			}

			for(unsigned y = bottom; y <= top; ++y)
				for(unsigned x = left; x <= right; ++x)
					if(y == (unsigned)(m*x + n))
						temp.push_back(Location(x,y));
		}
		Path path(temp);
		return path;
}

double Partical::probaByLazer(LaserProxy *lp, MapSearchable map){
	double hits = 0;
	for(uint i = 0; i < lp->GetCount(); ++i) {
		double d = (*lp)[i];
		double angle = this->position.getYaw() + lp->GetBearing(i);
		double xObs = this->position.getX() + d*cos(angle);
		double yObs = this->position.getY() + d*sin(angle);
		double virXObs = this->position.getX() + 2*d*cos(angle);
		double virYObs = this->position.getY() + 2*d*sin(angle);
		Path temp=linearPath(Location(this->position.getX(),this->position.getY()),Location(virXObs,virYObs));
		if (d < lp->GetMaxRange()){
			for (unsigned j = 1; j < temp.getPath().size(); ++j){
				if (map(temp.getPath()[j].getX(),temp.getPath()[j].getY())==255){
					double disntance = sqrt(pow(temp.getPath()[j].getX()-xObs,2)+pow(temp.getPath()[j].getY()-yObs,2));
					hits+=1-(atan(disntance)/(PI/2));
					break;
				}
			}
		}
		else{
			bool check=0;
			for (unsigned k = 1; k < temp.getPath().size()/2; ++k){
				if (map(temp.getPath()[k].getX(),temp.getPath()[k].getY())==255){
					double disntance = sqrt(pow(temp.getPath()[k].getX()-xObs,2)+pow(temp.getPath()[k].getY()-yObs,2));
					hits+=1-(atan(disntance)/(PI/2));
					check=1;
					break;
				}
			}
			if (!check)hits=+1;
		}
	}
	return hits/lp->GetCount();
}

double Partical::probaByMove(double dx, double dy, double dyaw){
	double disntance = sqrt(pow(dx,2)+pow(dy,2));
	//when atan is getting positive value it goes from 0 to PI/2, so we changed it to get values from 0 to 1
	//Also as the value is higer so is to value that atan returns and we changed it to get to opsitie values by decreassing it from 1.
	return 1-(atan(disntance+abs(dyaw))/(PI/2));
 }
