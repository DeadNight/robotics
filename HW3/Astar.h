/*
 * Astar.h
 *
 *  Created on: May 6, 2016
 *      Author: user
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include "Position.h"
#include "Map.h"
#include "Location.h"
#include "Solution.h"
#include "MapSearchable.h"

#include <cmath>
#include <vector>
#include <set>

class Astar{
	MapSearchable map;
	Position start;
	Location goal;
public:
	Astar(MapSearchable s);
	Position getStart();
	void setStart(Position start);
	Location getGoal();
	void setGoal(Location goal);
	 /**
		* @param state the checked state
		* @param goal the goal state
		* @return the sum of distance in all axis
		*/
	double h(Location state, Location goal);
	/**
		* @param s Astar type
		* @return Solution to the problem
		*/
	Solution search(Astar s);
};

#endif /* ASTAR_H_ */
