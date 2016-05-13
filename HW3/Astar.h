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
#include "Path.h"
#include "MapSearchable.h"
#include "State.h"

#include <cmath>
#include <vector>
#include <set>
#include <queue>

class Astar{
	MapSearchable mapSearchable;
	State start;
	State goal;
public:
	Astar(MapSearchable s);

	State getStart() const;
	void setStart(State start);

	State getGoal() const;
	void setGoal(State goal);

	MapSearchable getMapSearchable() const;
	void setMapSearchable(MapSearchable m);

	bool openListConatins(std::priority_queue<State> openList, State s);

	State toState(Location l);
	//return states of all position moves from a position in the maze
	std::vector<State> getAllPossibleStates(State state);

	 /**
		* @param state the checked state
		* @param goal the goal state
		* @return the sum of distance in all axis
		*/
	double manhattenDistance(State start,State goal);
	//get an array of all the moves for the solution and return as Path
	Path backTrace(State goalState, State startState);
	/**
		* @param s Astar type
		* @return Solution to the problem
		*/
	Path search();
};

#endif /* ASTAR_H_ */
