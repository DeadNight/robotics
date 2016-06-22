#include "Astar.h"


Astar::Astar(MapSearchable s) {
	setMapSearchable(s);
}

MapSearchable Astar::getMapSearchable() const {
	return mapSearchable;
}

void Astar::setMapSearchable(MapSearchable m) {
	mapSearchable = m;
}

//replace State in list with
Path Astar::search() {
	std::vector<State*> openList;
	std::vector<State*> closedList;

	openList.push_back(&mapSearchable.getStartState());
	std::push_heap(openList.begin(), openList.end(), State::Greater());

	while(!openList.empty()) {
		std::pop_heap(openList.begin(), openList.end(), State::Greater());
		State& current = *openList.back();
		openList.pop_back();

		closedList.push_back(&current);

		sanity(current);

		if(current == mapSearchable.getGoalState()) {
			Path solution = backTrace(current);
			cleanupStates();
			return solution;
		}

		std::vector<State*> successors = mapSearchable.getAllPossibleStates(current);
		states.insert(states.end(), successors.begin(), successors.end());

		while(!successors.empty()) {
			State& nextState = *successors.back();
			successors.pop_back();

			std::vector<State*>::iterator openIt = find(openList, nextState);
			std::vector<State*>::iterator closedIt = find(closedList, nextState);

			bool isNextStateOpen = (openIt != openList.end());
			bool isNextStateClosed = (closedIt != closedList.end());

			if(!isNextStateOpen && !isNextStateClosed) {
				openList.push_back(&nextState);
				std::push_heap(openList.begin(), openList.end(), State::Greater());
			} else if(isNextStateOpen) {
				State& openState = **openIt;
				if(nextState.getBaseCost() < openState.getBaseCost()) {
					openState.setBaseCost(nextState.getBaseCost());
					openState.setPrevState(current);

					std::make_heap(openList.begin(), openList.end(), State::Greater());
				}
			} else if(isNextStateClosed) {
				State& closedState = **closedIt;
				if(nextState.getBaseCost() < closedState.getBaseCost()) {
					closedList.erase(closedIt);

					closedState.setBaseCost(nextState.getBaseCost());
					closedState.setPrevState(current);

					openList.push_back(&closedState);
					std::push_heap(openList.begin(), openList.end(), State::Greater());
				}
			}
		}
	}
	cleanupStates();
	//if the loop ended there is no solution return empty
	return std::vector<Location>();
}

std::vector<State*>::iterator Astar::find(std::vector<State*>& list, const State& state) {
	for(std::vector<State*>::iterator it = list.begin(); it != list.end(); ++it)
		if(state == **it)
			return it;
	return list.end();
}

/**
 * get an array of all the moves for the solution and return as Solution
 *
 * @param goalState State the goal state of the problem
 * @param startState State the start state of the problem
 * @return Path the Solution of the problem
 */
Path Astar::backTrace(const State& endState) {
	std::vector<Location> locations;
	const State* state = &endState;

	while(state->getPrevState() != NULL) {
		locations.push_back(state->getLocation());
		state = state->getPrevState();
	}
	locations.push_back(mapSearchable.getStartState().getLocation());

	std::vector<Location> reverseLocations;
	for(std::vector<Location>::reverse_iterator it = locations.rbegin(); it != locations.rend(); ++it) {
		reverseLocations.push_back(*it);
	}

	return Path(reverseLocations);
}

void Astar::cleanupStates() {
	while(!states.empty()) {
		delete states.back();
		states.pop_back();
	}
}

void Astar::sanity(const State& endState) {
	return;
	std::vector<const State*> states;
	const State* state = &endState;

	while(state != NULL) {
		for(std::vector<const State*>::iterator it = states.begin(); it != states.end(); ++it) {
			if(state == *it) {
				std::cout << "!!! sanity failed on [" << state << "] (" << state->getLocation() << ")" << std::endl;
				throw "";
			}
		}
		states.push_back(state);
		state = state->getPrevState();
	}
}
