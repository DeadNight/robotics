/*
 * State.h
 *
 *  Created on: May 10, 2016
 *      Author: colman
 */

#ifndef STATE_H_
#define STATE_H_

#include <string>

#include "Location.h"
#include "MapSearchable.h"

class State {
	double baseCost;
	double locationCost;
	double heuristicCost;
	Location location;
	const State* prevState;
public:
	State();
	State(const Location& l);
	State(const Location& l, double bc, double lc, double hc);
	State(const Location& l, double bc, double lc, double hc, const State& prevState);

	double getBaseCost() const;
	void setBaseCost(double c);

	double getCost() const;

	Location getLocation()const;
	void setLocation(Location l);

	inline const double getX() const { return location.getX(); }
	inline const double getY() const { return location.getY(); }

	const State* getPrevState()const;
	void setPrevState(const State& s);

	double getLocationCost() const;
	void setLocationCost(double lc);

	//operator for the sort heap so it will get the minimum value of the vector
	inline bool operator<(const State& other) const { return getCost() < other.getCost(); }
	inline bool operator>(const State& other) const { return other < *this; }
	inline bool operator<=(const State& other) const { return !(*this > other); }
	inline bool operator>=(const State& other) const { return !(*this < other); }

	inline bool operator==(const State& other) const {
		return (getX() == other.getX()) && (getY() == other.getY());
	}
	inline bool operator!=(const State& other) const { return !(*this == other); }

	struct Greater {
		inline bool operator()(const State* lhs, const State* rhs) const { return *lhs > *rhs; }
	};
};

#endif /* STATE_H_ */
