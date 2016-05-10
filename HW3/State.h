/*
 * State.h
 *
 *  Created on: May 10, 2016
 *      Author: colman
 */

#ifndef STATE_H_
#define STATE_H_

#include "Location.h"
#include "MapSearchable.h"

class State: public Location {
	int cost;
	int wallDist;
public:
	State() { }
	State(Location l);
	State(State s);

	int getCost();
	void setCost(unsigned a);

	int getWallDist();
	void setWallDist(unsigned a);
};

#endif /* STATE_H_ */
