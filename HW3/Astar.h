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

#include <cmath>
#include <list>
#include <set>

class Astar{
	Map map;
	Position start;
	Location goal;
public:
	Astar(Map map, Position start, Location goal);
	Map getMap();
	void setMap(Map map);
	Position getStart();
	void setStart(Position start);
	Location getGoal();
	void setGoal(Location goal);
	 /**
		* @param state the checked state
		* @param goal the goal state
		* @return the sum of distance in all axis
		*/
	double h(Position state, Location goal);
	/**
		* @param s Astar type
		* @return Solution to the problem
		*/
	Solution search(Astar s);
};

#endif /* ASTAR_H_ */
