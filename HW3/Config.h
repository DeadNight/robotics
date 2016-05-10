/*
 * Config.h
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <iostream>
#include <string>
#include <cstring>
#include "Position.h"
#include "Location.h"
#include "Size.h"

class Config {
	char* mapFilePath;
	Position start;
	Location goal;
	Size robotSize;
	float mapResolution;
	float gridResoluton;
public:
	Config();
	virtual ~Config();

	char* getMapFilePath() const;
	void setMapFilePath(char* mapFilePath);

	Position getStart() const;
	Location getGoal() const;
	Size getRobotSize() const;

	float getMapResolution() const;
	void setMapResolution(float mapResolution);

	float getGridResolution() const;
	void setGridResolution(float gridResolution);

	friend std::ostream& operator<<(std::ostream& out, const Config& config);
	friend std::istream& operator>>(std::istream& in, Config& config);
};

#endif /* CONFIG_H_ */
