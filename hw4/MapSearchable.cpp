/*
* MapSearchable.cpp
*
*  Created on: May 4, 2016
*      Author: Nir Leibovitch 200315232
*      Author: Ron Naor 021615356
*      Author: Shay Kremer 201588126
*/

#include "MapSearchable.h"

MapSearchable::MapSearchable(Map map, Position start, Location goal) {
	setMap(map);

	this->start = map.getGridPosition(start);
	this->goal = map.getGridLocation(goal);

	this->startState = State(map.getWorldLocation(this->start.getLocation()), 0, this->operator()(this->start.getLocation()), airDistance(this->start.getLocation(), this->goal));
	this->goalState = State(map.getWorldLocation(this->goal));
}

Map MapSearchable::getMap() const {
	return map;
}

void MapSearchable::setMap(Map map) {
	this->map = map;

	unsigned width = getWidth();
	unsigned height = getHeight();

	grid.resize(width * height);
	for(unsigned i = 0; i < width * height; ++i)
		grid[i] = map[i] ? 255 : 0;
}

float MapSearchable::getGridResolution() const {
	return map.getGridResolution();
}

float MapSearchable::getMapResolution() const {
	return map.getMapResolution();
}

State& MapSearchable::getStartState() {
	return startState;
}

const State& MapSearchable::getStartState() const {
	return startState;
}

State& MapSearchable::getGoalState() {
	return goalState;
}

const State& MapSearchable::getGoalState() const {
	return goalState;
}

double MapSearchable::airDistance(const Location& l1, const Location& l2) const {
	double xAxis = std::pow(l1.getX() - l2.getX(), 2);
	double yAxis = std::pow(l1.getY() - l2.getY(), 2);
	return sqrt(xAxis + yAxis);
}

//return states of all position moves from a position
std::vector<State*> MapSearchable::getAllPossibleStates(const State& state) {
	std::vector<State*> possibleStates;
	std::vector<Location> locations;

	Location gridLocation = map.getGridLocation(state.getLocation());

	unsigned pX = gridLocation.getX();
	unsigned pY = gridLocation.getY();

	locations.push_back(Location(pX - 1, pY + 1));
	locations.push_back(Location(pX - 1, pY));
	locations.push_back(Location(pX - 1, pY - 1));
	locations.push_back(Location(pX, pY - 1));
	locations.push_back(Location(pX + 1, pY - 1));
	locations.push_back(Location(pX + 1, pY));
	locations.push_back(Location(pX + 1, pY + 1));
	locations.push_back(Location(pX, pY + 1));

	for(std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it) {
		Location& l = *it;
		if(l.getX() >= 0 && l.getY() >= 0 && l.getX() < getWidth() && l.getY() < getHeight() && this->operator()(l) != 1) {
			double stepCost = airDistance(l, gridLocation);
			double locationCost = 60 * stepCost * pow(this->operator()(l), 4);
			double heuristicCost = airDistance(l, goal);
			possibleStates.push_back(new State(map.getWorldLocation(l), state.getBaseCost() + stepCost, locationCost, heuristicCost, state));
		}
	}

	return possibleStates;
}

unsigned MapSearchable::getWidth() const {
	return map.getWidth();
}

unsigned MapSearchable::getHeight() const {
	return map.getHeight();
}

std::vector<unsigned char> MapSearchable::getGrid() const {
	return grid;
}

void MapSearchable::smooth(Size robotSize) {
	unsigned width = getWidth();
	unsigned height = getHeight();

	for(unsigned i = 0; i < width * height; ++i) {
		grid[i] = 0;
	}

	unsigned robotBiggestSize = robotSize.getWidth() > robotSize.getHeight() ? robotSize.getWidth() : robotSize.getHeight();
	unsigned smoothSize = ceil((robotBiggestSize / 2) / getGridResolution());
	smoothSize *= 3;

	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			unsigned pos = y*width + x;
			if (!map[pos])
				continue;

			for(unsigned i = 0; i < smoothSize; ++i) {
				for(unsigned j = 0; j < smoothSize; ++j) {
					if(x - j >= 0 && x + j < width && y - i >= 0 && y + i < height) {
						int xOffset = (int)j - (double)smoothSize/2;
						int yOffset = (int)i - (double)smoothSize/2;
						int smoothPos = (double)pos + yOffset*(int)width + xOffset;
						double factor = (double)(std::abs(yOffset) + std::abs(xOffset)) / smoothSize;
						unsigned color = 255 - (factor * 255);
						if(color > grid[smoothPos])
							grid[smoothPos] = color;
					}
				}
			}
		}
	}
}

void MapSearchable::save(const char* mapFilePath) const {
	toImage().save(mapFilePath);
}

Image MapSearchable::toImage() const {
	double resolutionRatio = (double)getGridResolution() / getMapResolution();
	unsigned pixelSize = resolutionRatio < 1 ? 1 : resolutionRatio;

	Image image(getWidth() * pixelSize, getHeight() * pixelSize);

	for (unsigned y = 0; y < getHeight(); ++y) {
		for (unsigned x = 0; x < getWidth(); ++x) {
			if(x == start.getX() && y == start.getY()) {
				image.setAround(x*resolutionRatio + pixelSize/2, y*resolutionRatio + pixelSize/2, pixelSize, Color::Green);
			} else if(x == goal.getX() && y == goal.getY()) {
				image.setAround(x*resolutionRatio + pixelSize/2, y*resolutionRatio + pixelSize/2, pixelSize, Color::Blue);
			} else {
				unsigned char value = 255 - grid[y*getWidth() + x];
				Color color(value, value, value);
				image.setAround(x*resolutionRatio + pixelSize/2, y*resolutionRatio + pixelSize/2, pixelSize, color);
			}
		}
	}
	return image;
}

void MapSearchable::colorPixel(std::vector<unsigned char>& image, double x, double y, unsigned pixelSize, Color color) {
	double imageWidth = getWidth() * pixelSize;

	for(unsigned i = 0; i < pixelSize; ++i) {
		for(unsigned j = 0; j < pixelSize; ++j) {
			unsigned imagePos = ((y*pixelSize)+i)*imageWidth*4 + ((x*pixelSize)+j)*4;
			image[imagePos + 0] = color.getRed();
			image[imagePos + 1] = color.getGreen();
			image[imagePos + 2] = color.getBlue();
			image[imagePos + 3] = color.getAlpha();
		}
	}
}

unsigned char MapSearchable::operator[](std::size_t i) {
	return grid[i];
}

const unsigned char MapSearchable::operator[](std::size_t i) const {
	return grid[i];
}

double MapSearchable::operator()(Location l) const {
	return this->operator()(l.getX(), l.getY());
}

double MapSearchable::operator()(unsigned x, unsigned y) const {
	if(x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) {
		std::cout << "out of bounds " << x << ", " << y << std::endl;
		throw "out of bounds";
	}
	return (double)grid[y*getWidth() + x]/255;
}

std::ostream& operator<<(std::ostream& out, const MapSearchable& searchable) {
	unsigned width = searchable.getWidth();
	unsigned height = searchable.getHeight();
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			unsigned pos = y*width + x;
			out << " ";
			if(y == searchable.start.getY() && x == searchable.start.getX()) {
				out << "S";
			} else if(y == searchable.goal.getY() && x == searchable.goal.getX()) {
				out << "G";
			} else if(searchable.grid[pos] == 255)
				out << "@";
			else if (searchable.grid[pos] > 200)
				out << "#";
			else if (searchable.grid[pos] > 150)
				out << "%";
			else if (searchable.grid[pos] > 100)
				out << "O";
			else if (searchable.grid[pos] > 50)
				out << "-";
			else if (searchable.grid[pos] > 0)
				out << ".";
			else // grid[pos] == 0
				out << " ";
		}
		out << std::endl;
	}
	return out;
}
