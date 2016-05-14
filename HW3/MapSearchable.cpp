/*
* MapSearchable.cpp
*
*  Created on: May 4, 2016
*      Author: Nir Leibovitch 200315232
*      Author: Ron Naor 021615356
*      Author: Shay Kremer 201588126
*/

#include "MapSearchable.h"

MapSearchable::MapSearchable(Map map, Position start, Location goal, double mapResolution) {
	setMap(map);
	setGrid(map);
	setStart(start, mapResolution);
	setGoal(goal, mapResolution);
}

Map MapSearchable::getMap() const {
	return map;
}

void MapSearchable::setMap(Map map) {
	this->map = map;
}

Position MapSearchable::getStart() const {
	return start;
}

void MapSearchable::setStart(Position start, double mapResolution) {
	int x = start.getLocation().getX() * mapResolution / map.getGridResolution();
	int y = start.getLocation().getY() * mapResolution / map.getGridResolution();
	Location location(x, y);
	Position newStart(location, start.getYaw());
	this->start = newStart;
}

Location MapSearchable::getGoal() const {
	return goal;
}

void MapSearchable::setGoal(Location goal, double mapResolution) {
	int x = goal.getX() * mapResolution / map.getGridResolution();
	int y = goal.getY() * mapResolution / map.getGridResolution();
	Location newGoal(x, y);
	this->goal = newGoal;
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
	unsigned smoothSize = ceil((robotBiggestSize / 2) / map.getGridResolution());
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

void MapSearchable::setGrid(Map map) {
	unsigned width = getWidth();
	unsigned height = getHeight();

	grid.resize(width * height);
	for(unsigned i = 0; i < width * height; ++i)
		grid[i] = map[i] ? 255 : 0;
}

void MapSearchable::save(const char* mapFilePath, float mapResolution) const {
	toImage(mapResolution).save(mapFilePath);
}

Image MapSearchable::toImage(float mapResolution) const {
	double resolutionRatio = (double)map.getGridResolution() / mapResolution;
	unsigned pixelSize = resolutionRatio < 1 ? 1 : resolutionRatio;

	Color green(0, 255, 0);
	Color blue(0, 0, 255);
	Image image(getWidth() * pixelSize, getHeight() * pixelSize);

	for (unsigned y = 0; y < getHeight(); ++y) {
		for (unsigned x = 0; x < getWidth(); ++x) {
			if(x == start.getX() && y == start.getY()) {
				image.setAround(x*resolutionRatio, y*resolutionRatio, pixelSize, green);
			} else if(x == goal.getX() && y == goal.getY()) {
				image.setAround(x*resolutionRatio, y*resolutionRatio, pixelSize, blue);
			} else {
				unsigned char value = 255 - grid[y*getWidth() + x];
				Color color(value, value, value);
				image.setAround(x*resolutionRatio, y*resolutionRatio, pixelSize, color);
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

const unsigned char MapSearchable::operator()(unsigned x, unsigned y) const {
	return grid[y*getWidth() + x];
}

std::ostream& operator<<(std::ostream& out, const MapSearchable& searchable) {
	unsigned width = searchable.getWidth();
	unsigned height = searchable.getHeight();
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			unsigned pos = y*width + x;
			out << " ";
			if(y == searchable.start.getLocation().getY() && x == searchable.start.getX()) {
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
