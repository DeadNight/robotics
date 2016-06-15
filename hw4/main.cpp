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
#include "Partical.h"
#include "LocalizationManager.h"
#include <libplayerc++/playerc++.h>

using namespace std;

int main() {
	Config config;
	ifstream configIn("/home/colman/robotics/PcBotWorld/parameters.txt");
	configIn >> config;
	configIn.close();

	cout << "config: " << endl << config << endl;

	double mapResolution = config.getMapResolution();
	double gridResolution = config.getGridResolution();

	Map map(gridResolution);
	map.load(config.getMapFilePath(), mapResolution);

	Map inflatedMap(map);
	inflatedMap.inflate(config.getRobotSize());

	map.save("map.png", config.getMapResolution());
	inflatedMap.save("inflatedMap.png", config.getMapResolution());

	MapSearchable searchable(inflatedMap, config.getStart(), config.getGoal(), mapResolution);
	searchable.smooth(config.getRobotSize());
	searchable.save("searchable.png", mapResolution);

	vector<Location> locations;

	Astar astar(searchable);
	Path path = astar.search();

	Solution solution(searchable, path);
	solution.save("solution.png", config.getMapResolution());

	path.reduce();

	Solution reducedSolution(searchable, path);
	reducedSolution.save("reducedSolution.png", config.getMapResolution());

	Position start(path[0], config.getStart().getYaw());
	Position end(path[path.size()-1],0);

	/*std::vector<bool> grid = map.getGrid();
	for (unsigned y = 0; y < map.getHeight(); ++y) {
			for (unsigned x = 0; x < map.getWidth(); ++x) {
				if (grid[y*map.getWidth() + x])
				cout<< x<<","<<y<<"= "<<grid[y*map.getWidth() + x]<<endl;
			}
	}
*/
	Partical particl(start, 1);
	cout << particl.getPosition() << endl << particl.getBelief() << endl;

	vector<Partical *> _particles;
	_particles.push_back(&particl);

	LocalizationManager local(_particles,map);

	PlayerClient *pc=  new PlayerClient("localhost",6665);

	LaserProxy *lp= new LaserProxy(pc);
	Position2dProxy *pp = new Position2dProxy(pc);

	pp->SetMotorEnable(true);
	pc->Read();
	pp->SetSpeed(0.1, 0);
	pc->Read();

	double num =1;
	local.update(lp,num,num,num);
	local.printParticels("particles.png",mapResolution);

	Robot robot("localhost", 6665, config.getRobotSize(), start, map);
	//robot.moveTo(path);


	cout << "Success" << endl;
	return 0;
}
