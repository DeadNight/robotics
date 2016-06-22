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

class Astar {
	MapSearchable mapSearchable;
	std::vector<State*> states;
public:
	Astar(MapSearchable s);

	MapSearchable getMapSearchable() const;
	void setMapSearchable(MapSearchable m);

	/*
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
