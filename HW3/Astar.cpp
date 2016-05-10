#include "Astar.h"


Astar::Astar(Map map, Position start, Location goal) {
	setMap(map);
	setStart(start);
	setGoal(goal);
}

Map Astar::getMap() {
	return map;
}

void Astar::setMap(Map map) {
	this->map = map;
}

Position Astar::getStart() {
	return start;
}

void Astar::setStart(Position start) {
	this->start = start;
}

Location Astar::getGoal() {
	return goal;
}

void Astar::setGoal(Location goal) {
	this->goal = goal;
}

double Astar::h(Position start,Location goal){
			double xAxis = (double)std::abs(start.getX()-goal.getX());
			double yAxis = (double)std::abs(start.getY()-goal.getY());
			return xAxis+yAxis;
			}


Solution Astar::search(Astar s){
	double temp;
	std::list<Position> openList;

	openList.insert(getStart);

	std::set <Position> closedSet;

	while(openList.size()>0)
	{
	Position current=openList.pop_front();// dequeue
	closedSet.insert(current);

	if(current.equals(s.getGoalState()))
	{
	return backTrace(current, s.getStart()); // private method, back traces through the parents
	}

	std::list<Position> successors=s.getAllPossibleStates(current);
	for(Position state : successors)
	{
	//check if the state is not in closedSet and openList
	if(!closedSet.contains(state) && !openListContains(state))
	{
	state.setCameFrom(current); //set the state camefrom to current
	temp=state.UpdateCost() + heuristic.h(current, s.getGoalState()); //calculate the g() (path until this point like in BFS) + h() (the path left from calc by heuristic)
	state.setCost(temp); //set the cost of the state
	openList.insert(state);
	}
	//check if the state is in openList and the cost of the current state is less then the cost of the state
	else if(openListContains(state))
	{
	State <T> tempState;
	Iterator<State <T>> it = openList.iterator();
	while (it.hasNext())
	{
	tempState=it.next();
	if (tempState.equals(state) && current.getCost()<tempState.getCameFrom().getCost())
	{
	openList.remove(state);
	state.setCameFrom(current); //set the state camefrom to current
	temp=state.UpdateCost() + heuristic.h(current, s.getGoalState()); //calculate the g() (path until this point like in BFS) + h() (the path left from calc by heuristic)
	state.setCost(temp); //set the cost of the state
	addToOpenList(state);
	break;
	}
	}
	}
	//check if the state is in closedSet and the cost of the current state is less then the cost of the state
	else if((closedSet.contains(state)))
	{
	State <T> tempState;
	Iterator<State <T>> it = closedSet.iterator();
	while (it.hasNext())
	{
	tempState=it.next();
	if (tempState.equals(state) && current.getCost()<tempState.getCameFrom().getCost())
	{
	state.setCameFrom(current); //set the state camefrom to current
	temp=state.UpdateCost() + heuristic.h(current, s.getGoalState()); //calculate the g() (path until this point like in BFS) + h() (the path left from calc by heuristic)
	state.setCost(temp); //set the cost of the state
	addToOpenList(state);
	closedSet.remove(state);
	break;
	}
	}
	}
	}
	}
	return null; //if the loop ended there is no solution return NULL
	}
}
