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
#include "LocalizationManager.h"
#include "Deltas.h"

using namespace std;

int main() {
	cout << "loading config... ";
	Config config;
	ifstream configIn("/home/user/robotics/PcBotWorld/parameters.txt");
	configIn >> config;
	configIn.close();
	cout << "done" << endl;

//	cout << "config: " << endl
//		   << config << endl;

	float mapResolution = config.getMapResolution();

	cout << "loading map... ";
	Map map(config.getGridResolution());
	map.load(config.getMapFilePath(), mapResolution);
//	cout << "done" << endl
//		 << "saving map... ";
	map.save("images/map.png");
	cout << "done" << endl;

//	cout << "inflating map... ";
	Map inflatedMap(map);
	inflatedMap.inflate(config.getRobotSize());
//	cout << "done" << endl;

//	cout << "saving inflated map... ";
	inflatedMap.save("images/inflatedMap.png");
//	cout << "done" << endl;

	Position start = config.getStart() * mapResolution;
	Location goal = config.getGoal() * mapResolution;

//	cout << "creating searchable... ";
	MapSearchable searchable(inflatedMap, start, goal);
//	cout << "done" << endl
//		 << "smoothing searchable... ";
	searchable.smooth(config.getRobotSize());
//	cout << "done" << endl
//		 << "saving searchable... ";
	searchable.save("images/searchable.png");
//	cout << "done" << endl;

	vector<Location> locations;

	cout << "searching path... " << flush;
	Astar astar(searchable);
	Path path = astar.search();
	cout << "done" << endl;

//	cout << "creating solution... ";
	Solution solution(searchable, path);
//	cout << "done" << endl
//		 << "saving solution... ";
	solution.save("images/solution.png");
//	cout << "done" << endl;

//	cout << "reducing path... ";
	path.reduce();
//	cout << "done" << endl;

//	cout << "creating reduced solution... ";
	Solution reducedSolution(searchable, path);

//	cout << "done" << endl
//		 << "saving reduced solution... ";
	reducedSolution.save("images/reducedSolution.png");
//	cout << "done" << endl;

//	cout << "creating localization manager... ";
	LocalizationManager localizationManager(start, &map);
//	cout << "done" << endl;

	cout << "connecting to robot... ";
	Robot robot("localhost", 6665, config.getRobotSize(), start);
//	cout << "setting robot path... ";
	robot.setPath(path);

//	cout << "done" << endl
//		 << "moving robot to goal... " << endl;

	int i = 0;
	while(true) {
		//cout << '\t' << "reading robot sensors... ";
		robot.read();

		if(i % 5 == 0) {
			//cout << "getting deltas... " << flush;
			Deltas deltas = robot.getDeltas();

			//cout << "done" << endl
			//	 << '\t' << "updating localization manager... " << flush;
			const Position& approxPosition = localizationManager.update(robot.getLaserProxy(), deltas);

			//cout << "done" << endl
			//	 << '\t' << "setting robot position (" << approxPosition << ")... " << flush;
			robot.setPosition(approxPosition);
			//cout << "done" << endl;
		}

		if(robot.isAt(path[path.size() - 1]))
			break;

		//cout<< '\t' << "moving robot... ";
		robot.move();
		//cout << "done" << endl;

		if(i % 20 == 0) {
			cout << " robot is at " << robot.getPosition().getLocation()
				 << " facing " << robot.getPosition().getYaw() << endl;

			char filename[23];
			sprintf(filename, "images/%d_position.png", (int)(i/20));
			localizationManager.getBestParticle().save(filename);

			sprintf(filename, "images/%d_particles.png", (int)(i/20));
			localizationManager.save(filename);
		}

		i++;
	}

	cout << "robot finished at " << robot.getPosition().getLocation() << endl;

	return 0;
}
