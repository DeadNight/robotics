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

class Map {
	float gridResolution;
	std::vector<bool> grid;
	unsigned width, height;
public:
	Map() { }
	Map(float gridResolution);

	float getGridResolution() const;
	void setGridResolution(float gridResolution);

	std::vector<bool> getGrid() const;
	unsigned getWidth() const;
	unsigned getHeight() const;

	void load(char* mapFilePath, float mapResolution);
	void save(char* mapFilePath, float mapResolution) const;

	Image toImage(float mapResolution) const;

	void inflate(Size robotSize);

	friend std::ostream& operator<<(std::ostream& out, const Map& map);

	unsigned char operator[](std::size_t i);
	const unsigned char operator[](std::size_t i) const;
};

#endif /* MAP_H_ */
