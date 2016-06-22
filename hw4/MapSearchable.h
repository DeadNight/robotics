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
#include "State.h"
#include "Color.h"

class MapSearchable {
	std::vector<unsigned char> grid;
	Map map;
	Position start;
	Location goal;

	State startState;
	State goalState;
public:
	MapSearchable() { }
	MapSearchable(Map map, Position start, Location goal);

    Map getMap() const;
    void setMap(Map map);

    float getGridResolution() const;
    float getMapResolution() const;

    State& getStartState();
	const State& getStartState() const;
	State& getGoalState();
	const State& getGoalState() const;

	bool isGoal(const State& state) const;

	// return the air distance
	double airDistance(const Location& l1, const Location& l2) const;

	// return states of all position moves from a position in the maze
	std::vector<State*> getAllPossibleStates(const State& state);

    unsigned getWidth() const;
    unsigned getHeight() const;
    std::vector<unsigned char> getGrid() const;

    void smooth(Size robotSize);

	void save(const char* mapFilePath) const;

	Image toImage() const;

	void colorPixel(std::vector<unsigned char>& image, double x, double y, unsigned pixelSize, Color color);

	unsigned char operator[](std::size_t i);
	const unsigned char operator[](std::size_t i) const;
	double operator()(Location l) const;
	double operator()(unsigned x, unsigned y) const;

	friend std::ostream& operator<<(std::ostream& out, const MapSearchable& searchable);
};

#endif /* MAPSEARCHABLE_H_ */
