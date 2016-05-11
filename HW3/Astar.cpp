#include "Astar.h"


Astar::Astar(MapSearchable s) {
	setStart(toState(s.getStart().getLocation()));
	setGoal(toState(s.getGoal()));
	setMapSearchable(s);
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

MapSearchable Astar::getMapSearchable() const{
	return mapSearchable;
}

void Astar::setMapSearchable(MapSearchable m){
	this->mapSearchable = m;
}

State Astar::toState(Location l){
	State s(l,mapSearchable(l.getX(),l.getY()));
	return s;
}

double Astar::manhattenDistance(State start,State goal){
	double xAxis = (double)std::abs(start.getLocation().getX()-goal.getLocation().getX());
	double yAxis = (double)std::abs(start.getLocation().getY()-goal.getLocation().getY());;
	return xAxis+yAxis;
}

/**
* @param goalState State the goal state of the problem
* @param startState State the start state of the problem
* @return Path the Solution of the problem
*/
Path Astar::backTrace(State goalState, State startState) { //get an array of all the moves for the solution and return as Solution
std::vector<Location> pathArr;
int cnt=0;
State state = goalState;
//counting all the steps for solution except for the start
while (!(state.getPrevState()==NULL)) {
	cnt++;
	state=*state.getPrevState();
}
pathArr.resize(cnt+1);
state = goalState;

while (!(state.getPrevState()==NULL))
{
pathArr[cnt]=state.getLocation();
state = *state.getPrevState();
cnt--;
}
//adding the start location that wasn't in the while loop
pathArr[cnt]=startState.getLocation();
return Path(pathArr);
}

std::set<State> Astar::getAllPossibleStates(State state) { //return states of all position moves from a position in the maze

std::set<State> possibleStates;

// get the reference point x y and z
unsigned char pX = state.getLocation().getX();
unsigned char pY = state.getLocation().getY();

if ((pX-1 >= 0) && (mapSearchable(pX-1,pY)!=255)) //Checks whether it is possible to move left
{
possibleStates.insert(State(Location(pX-1,pY),mapSearchable(pX-1,pY)));
}
if ((pX+1 < (unsigned char)mapSearchable.getHeight()) && (mapSearchable(pX+1,pY)!=255)) //Checks whether it is possible to move Right
{
	possibleStates.insert(State(Location(pX+1,pY),mapSearchable(pX+1,pY)));
}
if ((pY-1 >= 0) && (mapSearchable(pX,pY-1)!=255)) //Checks whether it is possible to move Down
{
	possibleStates.insert(State(Location(pX,pY-1),mapSearchable(pX,pY-1)));
}
if ((pY+1 < (unsigned char)mapSearchable.getWidth()) && (mapSearchable(pX,pY+1)!=255)) //Checks whether it is possible to move Up
{
	possibleStates.insert(State(Location(pX,pY+1),mapSearchable(pX,pY+1)));
}

return possibleStates;
}

Path Astar::search(){

	double temp;

	std::priority_queue<State> openList;
	openList.push(this->start);

	std::set<State> closedSet;

	while(!openList.empty())
	{
	State current=openList.top();
	openList.pop();// dequeue

	closedSet.insert(current);

	if(current==this->goal)
	{

	return backTrace(current,start); // private method, back traces through the parents
	}
	std::set<State> successors=getAllPossibleStates(current);
	for (std::set<State>::iterator i = successors.begin(); i != successors.end(); i++) {
	State temp = *i;
	//check if the state is not in closedSet and openList
	if(!closedSet.find(temp) && !(openList.top()==temp))
	{
	state.setPrevState(*i); //set the state camefrom to current
	temp=*i.UpdateCost() + manhattenDistance(current, start); //calculate the g() (path until this point like in BFS) + h() (the path left from calc by heuristic)
	*i.setCost(temp); //set the cost of the state
	openList.push(*i);
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
