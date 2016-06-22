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
	cout << "loading config... ";
	Config config;
	ifstream configIn("/home/user/robotics/PcBotWorld/parameters.txt");
	configIn >> config;
	configIn.close();
	cout << "done" << endl;

	cout << "config: " << endl
	     << config << endl;

	float mapResolution = config.getMapResolution();

	cout << "loading map... ";
	Map map(config.getGridResolution());
	map.load(config.getMapFilePath(), mapResolution);
	cout << "done" << endl
		 << "saving map... ";
	map.save("map.png");
	cout << "done" << endl;

	cout << "inflating map... ";
	Map inflatedMap(map);
	inflatedMap.inflate(config.getRobotSize());
	cout << "done" << endl;

	cout << "saving inflated map... ";
	inflatedMap.save("inflatedMap.png");
	cout << "done" << endl;

	Position start = config.getStart() * mapResolution;
	Location goal = config.getGoal() * mapResolution;

	cout << "creating searchable... ";
	MapSearchable searchable(inflatedMap, start, goal);
	cout << "done" << endl
		 << "smoothing searchable... ";
	searchable.smooth(config.getRobotSize());
	cout << "done" << endl
		 << "saving searchable... ";
	searchable.save("searchable.png");
	cout << "done" << endl;

	vector<Location> locations;

	cout << "searching path... ";
	cout.flush();
	Astar astar(searchable);
	Path path = astar.search();
	cout << "done" << endl;

	cout << "creating solution... ";
	Solution solution(searchable, path);
	cout << "done" << endl
		 << "saving solution... ";
	solution.save("solution.png");
	cout << "done" << endl;

	cout << "reducing path... ";
	path.reduce();
	cout << "done" << endl;

	cout << "creating reduced solution... ";
	Solution reducedSolution(searchable, path);

	cout << "done" << endl
		 << "saving reduced solution... ";
	reducedSolution.save("reducedSolution.png");
	cout << "done" << endl;

	cout << "connecting to robot... ";
	Robot robot("localhost", 6665, config.getRobotSize(), start);
	cout << "moving robot to goal... ";
	robot.moveTo(path);
	cout << "done" << endl;

/*
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
*/

	cout << "Success" << endl;
	return 0;
}
