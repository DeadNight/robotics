#include "Astar.h"


Astar::Astar(MapSearchable s) {
	start = s.getStart();
	goal = s.getGoal();
	map = s;
}

Position Astar::getStart() {
	return start;
}

void Astar::setStart(Position start) {
	this->start = start;
}

Location Astar::getGoal() {
	return goal;
}

void Astar::setGoal(Location goal) {
	this->goal = goal;
}

double Astar::h(Location start,Location goal){
			return sqrt(pow(start.getX() - goal.getX(), 2) + pow(start.getY() - goal.getY(), 2));
			}


Solution Astar::search(Astar s){

}
