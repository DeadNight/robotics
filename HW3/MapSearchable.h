/*
 * MapSearchable.h
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#ifndef MAPSEARCHABLE_H_
#define MAPSEARCHABLE_H_

#include <vector>
#include "Map.h"
#include "Position.h"
#include "Location.h"
#include "Color.h"

class MapSearchable {
	std::vector<unsigned char> grid;
	Map map;
	Position start;
	Location goal;
public:
	MapSearchable() { }
	MapSearchable(Map map, Position start, Location goal, double mapResolution);

    Map getMap() const;
    void setMap(Map map);

    Position getStart() const;
    void setStart(Position start, double mapResolution);

    Location getGoal() const;
    void setGoal(Location goal, double mapResolution);

    unsigned getWidth() const;
    unsigned getHeight() const;
    std::vector<unsigned char> getGrid() const;

    void smooth(Size robotSize);

	void save(char* mapFilePath, float mapResolution) const;

	Image toImage(float mapResolution) const;

	void colorPixel(std::vector<unsigned char>& image, double x, double y, unsigned pixelSize, Color color);

	unsigned char operator[](std::size_t i);
	const unsigned char operator[](std::size_t i) const;
private:
	void setGrid(Map map);
public:
	friend std::ostream& operator<<(std::ostream& out, const MapSearchable& searchable);
};

#endif /* MAPSEARCHABLE_H_ */
