/*
 * Map.cpp
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <cmath>
#include "Size.h"
#include "Image.h"
#include "Location.h"
#include "Position.h"

class Map {
	float gridResolution;
	float mapResolution;
	std::vector<bool> grid;
	unsigned width, height;
public:
	Map() { }
	Map(float gridResolution);

	float getGridResolution() const;
	void setGridResolution(float gridResolution);

	float getMapResolution() const;

	std::vector<bool> getGrid() const;
	unsigned getWidth() const;
	unsigned getHeight() const;

	Position getGridPosition(Position position) const;
	Location getGridLocation(Location location) const;

	Location getWorldLocation(Location location) const;

	void load(const char* mapFilePath, float mapResolution);
	void save(const char* mapFilePath) const;

	Image toImage() const;

	void inflate(Size robotSize);

	friend std::ostream& operator<<(std::ostream& out, const Map& map);

	unsigned char operator[](std::size_t i);
	const unsigned char operator[](std::size_t i) const;

	double operator()(Location l) const;
	double operator()(unsigned x, unsigned y) const;
};

#endif /* MAP_H_ */
