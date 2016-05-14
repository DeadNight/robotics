#include "Astar.h"


Astar::Astar(MapSearchable s) {
	setMapSearchable(s);
	setStart(toState(s.getStart().getLocation()));
	setGoal(toState(s.getGoal()));
}

State Astar::getStart() const{
	return start;
}

void Astar::setStart(State start) {
	this->start = start;
}

State Astar::getGoal() const{
	return goal;
}

void Astar::setGoal(State goal) {
	this->goal = goal;
}

MapSearchable Astar::getMapSearchable() const {
	return mapSearchable;
}

void Astar::setMapSearchable(MapSearchable m) {
	this->mapSearchable = m;
}

State Astar::toState(Location l) {
	State s(l, mapSearchable(l.getX(),l.getY()));
	return s;
}

double Astar::manhattenDistance(State start,State goal) {
	double xAxis = (double)std::abs(start.getLocation().getX() - goal.getLocation().getX());
	double yAxis = (double)std::abs(start.getLocation().getY() - goal.getLocation().getY());;
	return xAxis + yAxis;
}

/**
 * get an array of all the moves for the solution and return as Solution
 *
 * @param goalState State the goal state of the problem
 * @param startState State the start state of the problem
 * @return Path the Solution of the problem
 */
Path Astar::backTrace(State goalState, State startState) {
	std::vector<Location> pathArr;
	int cnt = 0;
	State state = goalState;
	//counting all the steps for solution except for the start
	while (!(state.getPrevState() == NULL)) {
		cnt++;
		state = *state.getPrevState();
	}
	pathArr.resize(cnt + 1);
	state = goalState;

	while (!(state.getPrevState() == NULL)) {
		pathArr[cnt] = state.getLocation();
		state = *state.getPrevState();
		cnt--;
	}
	//adding the start location that wasn't in the while loop
	pathArr[cnt] = startState.getLocation();
	return Path(pathArr);
}

//return states of all position moves from a position in the maze
std::vector<State> Astar::getAllPossibleStates(State state) {
	std::vector<State> possibleStates;
	possibleStates.resize(4);
	// get the reference point x y and z
	unsigned  pX = state.getLocation().getX();
	unsigned  pY = state.getLocation().getY();

	//Checks whether it is possible to move left
	if ((pX-1 >= 0) && (mapSearchable(pX-1,pY)!=255)) {
		possibleStates.push_back(toState(Location(pX-1,pY)));
	}
	//Checks whether it is possible to move Right
	if ((pX+1 < (unsigned)mapSearchable.getHeight()) && (mapSearchable(pX+1,pY)!=255)) {
		possibleStates.push_back(toState(Location(pX,pY+1)));
	}
	//Checks whether it is possible to move Down
	if ((pY-1 >= 0) && (mapSearchable(pX,pY-1)!=255)) {
		possibleStates.push_back(toState(Location(pX,pY+1)));
	}
	//Checks whether it is possible to move Up
	if ((pY + 1 < (unsigned)mapSearchable.getWidth()) && (mapSearchable(pX, pY + 1) != 255)) {
		possibleStates.push_back(toState(Location(pX , pY + 1)));
	}
	return possibleStates;
}
//checks if certain state is in priority queue, if it is, it returns true
bool Astar::openListConatins(std::priority_queue<State> openList, State s) {
	std::set<State> temp;
	while (!openList.empty()) {
		temp.insert(openList.top());
		openList.pop();
	}
	return (!(temp.find(s) == temp.end()));
}

//replace State in list with
Path Astar::search() {
	std::priority_queue<State> openList;
	openList.push(this->start);

	std::set<State> closedSet;

	int num = 1;
	while(!openList.empty()) {
		num++;
		State current = openList.top();
		// dequeue
		openList.pop();
		closedSet.insert(current);

		if(current.eq(current,this->goal)) {
			return backTrace(current,start);
		}

		std::vector<State> successors = getAllPossibleStates(current);
		while(!successors.empty()) {
			State closedSetCheck = *closedSet.find(successors.back());
			State closedSetEnd = *closedSet.end();
			//check if the state is not in closedSet and openList
			if(current.eq(closedSetCheck, closedSetEnd) && !(openListConatins(openList, successors.back()))) {
				//set the state camefrom to current
				successors.back().setPrevState(&current);
				//calculate the g() (path until this point like in BFS) + h() (the path left from calc by heuristic)
				//and set the cost of the state
				successors.back().setCost(successors.back().UpdatedCost() + manhattenDistance(current, this->goal));
				openList.push(successors.back());
			}
			//check if the state is in openList and the cost of the current state is less then the cost of the state
			else if(openListConatins(openList, successors.back()) && current.getCost() < successors.back().getPrevState()->getCost()) {
				//set the state camefrom to current
				successors.back().setPrevState(&current);
				//calculate the g() (path until this point like in BFS) + h() (the path left from calc by heuristic)
				//and set the cost of the state
				successors.back().setCost(successors.back().UpdatedCost() + manhattenDistance(current, this->goal));
				openList.push(successors.back());
				successors.pop_back();
			}
			//check if the state is in closedSet and the cost of the current state is less then the cost of the state
			else if(!(current.eq(closedSetCheck, closedSetEnd)) && current.getCost() < successors.back().getPrevState()->getCost()) {
				//set the state camefrom to current
				successors.back().setPrevState(&current);
				//calculate the g() (path until this point like in BFS) + h() (the path left from calc by heuristic)
				//and set the cost of the state
				successors.back().setCost(successors.back().UpdatedCost() + manhattenDistance(current, this->goal));
				openList.push(successors.back());
				closedSet.erase(successors.back());
			}
		}
		//if the loop ended there is no solution return NULL
		std::vector<Location> null;
		return null;
	}
}
