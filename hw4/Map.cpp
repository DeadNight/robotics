/*
 * Map.cpp
 *
 *  Created on: May 4, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#include "Map.h"

Map::Map(float gridResolution) : gridResolution(gridResolution) {}

float Map::getGridResolution() const {
	return gridResolution;
}

void Map::setGridResolution(float gridResolution) {
	this->gridResolution = gridResolution;
}

float Map::getMapResolution() const {
	return mapResolution;
}

std::vector<bool> Map::getGrid() const {
	return grid;
}

unsigned Map::getWidth() const {
	return width;
}

unsigned Map::getHeight() const {
	return height;
}

Position Map::getGridPosition(Position position) const {
	Position temp = position / gridResolution;
	return Position(round(temp.getX()), round(temp.getY()), position.getYaw());
}

Location Map::getGridLocation(Location location) const {
	Location temp = location / gridResolution;
	return Location(round(temp.getX()), round(temp.getY()));
}

Location Map::getWorldLocation(Location location) const {
	Location temp = location * gridResolution;
	return Location(round(temp.getX()), round(temp.getY()));
}

void Map::load(const char* mapFilePath, float mapResolution) {
	this->mapResolution = mapResolution;

	Image image; //the raw image
	image.load(mapFilePath);

	double resolutionRatio = (double)gridResolution / mapResolution;
	unsigned pixelSize = resolutionRatio < 1 ? 1 : resolutionRatio;

	width = image.getWidth() / resolutionRatio;
	height = image.getHeight() / resolutionRatio;

	grid.resize(width * height);

	for(unsigned i = 0; i < width * height; ++i) {
		grid[i] = false;
	}

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			bool colored = false;
			for(unsigned i = 0; i < pixelSize; ++i) {
				for(unsigned j = 0; j < pixelSize; ++j) {
					Color color = image.getPixel(x*resolutionRatio + j, y*resolutionRatio + i);
					if(!(color.getRed() || color.getGreen() || color.getBlue())) {
						grid[y*width + x] = true;
						colored = true;
						break;
					}
				}
				if(colored)
					break;
			}
		}
	}
}

void Map::save(const char* mapFilePath) const {
	toImage().save(mapFilePath);
}

Image Map::toImage() const {
	double resolutionRatio = (double)gridResolution / mapResolution;
	unsigned pixelSize = resolutionRatio < 1 ? 1 : resolutionRatio;

	Image image(width * pixelSize, height * pixelSize);

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			if(grid[y*width + x]) {
				image.setAround(x*resolutionRatio + pixelSize/2, y*resolutionRatio + pixelSize/2, pixelSize, Color::Black);
			} else {
				image.setAround(x*resolutionRatio + pixelSize/2, y*resolutionRatio + pixelSize/2, pixelSize, Color::White);
			}
		}
	}
	return image;
}

void Map::inflate(Size robotSize) {
	std::vector<unsigned char> gridCopy;
	gridCopy.resize(width * height);

	for(unsigned i = 0; i < width * height; ++i) {
		gridCopy[i] = grid[i];
		grid[i] = false;
	}

	unsigned robotBiggestSize = robotSize.getWidth() > robotSize.getHeight() ? robotSize.getWidth() : robotSize.getHeight();
	unsigned inflateSize = ceil((robotBiggestSize / 2) / gridResolution);

	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			unsigned pos = y*width + x;
			if (!gridCopy[pos])
				continue;

			for(unsigned i = 0; i < inflateSize; ++i) {
				for(unsigned j = 0; j < inflateSize; ++j) {
					if(x - j >= 0 && x + j < width && y - i >= 0 && y + i < height) {
						int xOffset = (int)j - (double)inflateSize/2;
						int yOffset = (int)i - (double)inflateSize/2;
						int inflatePos = (double)pos + yOffset*(int)width + xOffset;
						if(inflatePos >= 0 && inflatePos <= width * height) {
							if(inflatePos >= 0 && inflatePos <= width * height) {
								grid[inflatePos] = true;
							}
						}
					}
				}
			}
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Map& map) {
	for (unsigned y = 0; y < map.height; ++y) {
		for (unsigned x = 0; x < map.width; ++x) {
			out << " ";
			if(map.grid[y*map.width + x])
				out << "@";
			else
				out << " ";
		}
		out << std::endl;
	}
	return out;
}

unsigned char Map::operator[](std::size_t i) {
	return grid[i];
}

const unsigned char Map::operator[](std::size_t i) const {
	return grid[i];
}
