#include "Astar.h"


Astar::Astar(MapSearchable s) {
	setMapSearchable(s);

	Location startLocation = s.getStart().getLocation();
	Location endLocation = s.getGoal();

	setStart(State(startLocation, 0, mapSearchable(startLocation.getX(), startLocation.getY()), airDistance(startLocation, endLocation)));
	setGoal(State(endLocation));
}

const State& Astar::getStart() const{
	return start;
}

void Astar::setStart(const State& start) {
	this->start = start;
}

const State& Astar::getGoal() const{
	return goal;
}

void Astar::setGoal(const State& goal) {
	this->goal = goal;
}

MapSearchable Astar::getMapSearchable() const {
	return mapSearchable;
}

void Astar::setMapSearchable(MapSearchable m) {
	this->mapSearchable = m;
}

double Astar::manhattenDistance(const Location& l1, const Location& l2) const {
	double xAxis = std::abs(l1.getX() - l2.getX());
	double yAxis = std::abs(l1.getY() - l2.getY());
	return xAxis + yAxis;
}

double Astar::airDistance(const Location& l1, const Location& l2) const {
	double xAxis = std::pow(l1.getX() - l2.getX(), 2);
	double yAxis = std::pow(l1.getY() - l2.getY(), 2);
	return sqrt(xAxis + yAxis);
}

//return states of all position moves from a position in the maze
std::vector<State*> Astar::getAllPossibleStates(const State& state) {
	std::vector<State*> possibleStates;

	unsigned pX = state.getX();
	unsigned pY = state.getY();

	Location l;

	//Checks whether it is possible to move left
	l = Location(pX - 1, pY);
	if (pX - 1 >= 0 && mapSearchable(l) != 255) {
		states.push_back(new State(l, state.getBaseCost() + airDistance(l, state.getLocation()), mapSearchable(l)/255, airDistance(l, this->goal.getLocation()), state));
		possibleStates.push_back(states.back());
	}
	//Checks whether it is possible to move Right
	l = Location(pX + 1, pY);
	if (pX + 1 < mapSearchable.getWidth() && mapSearchable(l) != 255) {
		states.push_back(new State(l, state.getBaseCost() + airDistance(l, state.getLocation()), mapSearchable(l)/255, airDistance(l, this->goal.getLocation()), state));
		possibleStates.push_back(states.back());
	}
	//Checks whether it is possible to move Down
	l = Location(pX, pY - 1);
	if (pY - 1 >= 0 && mapSearchable(l) != 255) {
		states.push_back(new State(l, state.getBaseCost() + airDistance(l, state.getLocation()), mapSearchable(l)/255, airDistance(l, this->goal.getLocation()), state));
		possibleStates.push_back(states.back());
	}
	//Checks whether it is possible to move Up
	l = Location(pX, pY + 1);
	if (pY + 1 < mapSearchable.getHeight() && mapSearchable(l) != 255) {
		states.push_back(new State(l, state.getBaseCost() + airDistance(l, state.getLocation()), mapSearchable(l)/255, airDistance(l, this->goal.getLocation()), state));
		possibleStates.push_back(states.back());
	}
	return possibleStates;
}

//replace State in list with
Path Astar::search() {
	std::vector<State*> openList;
	std::vector<State*> closedList;

	openList.push_back(&this->start);
	std::push_heap(openList.begin(), openList.end(), State::Greater());

	while(!openList.empty()) {
		std::pop_heap(openList.begin(), openList.end(), State::Greater());
		State& current = *openList.back();
		openList.pop_back();

		closedList.push_back(&current);

		sanity(current);

		if(current == this->goal) {
			Path solution = backTrace(current);
			cleanupStates();
			return solution;
		}

		std::vector<State*> successors = getAllPossibleStates(current);

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
	locations.push_back(this->start.getLocation());
	return Path(locations);
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
