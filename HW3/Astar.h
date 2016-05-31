/*
 * Astar.h
 *
 *  Created on: May 6, 2016
 *      Author: user
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include <cmath>
#include <vector>
#include <tr1/unordered_set>
#include <queue>
#include <iostream>

#include "Position.h"
#include "Map.h"
#include "Location.h"
#include "Path.h"
#include "MapSearchable.h"
#include "State.h"

class Astar{
	MapSearchable mapSearchable;
	State start;
	State goal;
	std::vector<State*> states;
public:
	Astar(MapSearchable s);

	const State& getStart() const;
	void setStart(const State& start);

	const State& getGoal() const;
	void setGoal(const State& goal);

	MapSearchable getMapSearchable() const;
	void setMapSearchable(MapSearchable m);

	//return states of all position moves from a position in the maze
	std::vector<State*> getAllPossibleStates(const State& state);

	 /**
		* @param state the checked state
		* @param goal the goal state
		* @return the sum of distance in all axis
		*/
	double manhattenDistance(const Location& l1, const Location& l2) const;
	double airDistance(const Location& l1, const Location& l2) const;

	/**
		* @param s Astar type
		* @return Solution to the problem
		*/
	Path search();

private:
	std::vector<State*>::iterator find(std::vector<State*>& list, const State& state);
	//get an array of all the moves for the solution and return as Path
	Path backTrace(const State& state);
	void cleanupStates();
	void sanity(const State& endState);
};

#endif /* ASTAR_H_ */
