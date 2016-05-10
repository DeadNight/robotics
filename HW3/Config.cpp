/*
 * Config.cpp
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#include "Config.h"

Config::Config() {
	mapFilePath = new char[1];
	mapFilePath[0] = '\n';
}

Config::~Config() {
	delete[] mapFilePath;
}

char* Config::getMapFilePath() const {
	return mapFilePath;
}
void Config::setMapFilePath(char* mapFilePath) {
	delete[] this->mapFilePath;
	this->mapFilePath = new char[strlen(mapFilePath) + 1];
	strcpy(this->mapFilePath, mapFilePath);
}

Position Config::getStart() const {
	return start;
}

Location Config::getGoal() const {
	return goal;
}

Size Config::getRobotSize() const {
	return robotSize;
}

float Config::getMapResolution() const {
	return mapResolution;
}
void Config::setMapResolution(float mapResolution) {
	this->mapResolution = mapResolution;
}

float Config::getGridResolution() const {
	return gridResoluton;
}
void Config::setGridResolution(float gridResolution) {
	this->gridResoluton = gridResolution;
}

std::ostream& operator<<(std::ostream& out, const Config& config) {
	return out << "map: " << config.mapFilePath << std::endl
		<< "startLocation: " << config.start << std::endl
		<< "goal: " << config.goal << std::endl
		<< "robotSize: " << config.robotSize << std::endl
		<< "MapResolutionCM: " << config.mapResolution << std::endl
		<< "GridResolutionCM: " << config.gridResoluton << std::endl;
}

std::istream& operator>>(std::istream& in, Config& config) {
	char label[20];

	// read label "map: "
	in.read(label, strlen("map: "));
	// read map file path
	char path[100];
	in >> path;
	config.setMapFilePath(path);

	// read label "startLocation: "
	in.read(label, strlen("startLocation: ") + 1);
	// read start location
	in >> config.start;

	// read label "goal: "
	in.read(label, strlen("goal: ") + 1);
	// read goal location
	in >> config.goal;

	// read label "robotSize: "
	in.read(label, strlen("robotSize: ") + 1);
	// read robot size
	in >> config.robotSize;

	// read label "MapResolutionCM: "
	in.read(label, strlen("MapResolutionCM: ") + 1);
	in >> config.mapResolution;

	// read label "GridResolutionCM: "
	in.read(label, strlen("GridResolutionCM: ") + 1);
	in >> config.gridResoluton;

	return in;
}
