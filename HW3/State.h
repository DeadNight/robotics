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

class State {
	double cost;
	unsigned char wallDist;
	Location location;
	State* _prevState;
public:
	State() { }
	State(Location l, unsigned char wd);

	~State();

	double getCost() const;
	void setCost(double a);

	Location getLocation()const;
	void setLocation(Location l);

	State* getPrevState()const;
	void setPrevState(State* _l);

	unsigned char getWallDist() const;
	void setWallDist(unsigned char wallDist);

//operator for the sort heap so it will get the minimum value of the vector
	friend bool operator < (const State& l, const State& r);

	bool eq(State s1, State s2);

	double UpdatedCost();
};

#endif /* STATE_H_ */
