/*
 * main.cpp
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "Config.h"
#include "Map.h"
#include "MapSearchable.h"
#include "Path.h"
#include "Location.h"
#include "Solution.h"
#include "Robot.h"
#include "Astar.h"

using namespace std;

int main() {
	Config config;
	ifstream configIn("/home/user/robotics/PcBotWorld/parameters.txt");
	configIn >> config;
	configIn.close();

	cout << "config: " << endl << config << endl;

	double mapResolution = config.getMapResolution();
	double gridResolution = config.getGridResolution();

	Map map(gridResolution);
	map.load(config.getMapFilePath(), mapResolution);
	map.inflate(config.getRobotSize());
	map.save("map.png", config.getMapResolution());

	MapSearchable searchable(map, config.getStart(), config.getGoal(), mapResolution);
	searchable.smooth(config.getRobotSize());
	searchable.save("searchable.png", mapResolution);

	vector<Location> locations;

	Astar astar(searchable);
	Path path = astar.search();

	path.reduce();

	Solution solution(searchable, path);
	solution.save("solution.png", config.getMapResolution());

	Position start(path[0], config.getStart().getYaw());

	Robot robot("localhost", 6665, config.getRobotSize(), start, map);
	robot.moveTo(path);

	cout << "Success" << endl;
	return 0;
}
