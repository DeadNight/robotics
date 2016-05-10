/*
 * Image.h
 *
 *  Created on: May 7, 2016
 *      Author: user
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <vector>
#include "lodepng.h"
#include "Color.h"
#include "Size.h"
#include "Location.h"

class Image {
	std::vector<Color> pixels;
	unsigned width, height;
public:
	Image() : width(0), height(0) { };
	Image(unsigned width, unsigned height);
	Image(Size size);

	void setSize(Size size);
	Size getSize() const;

	void setWidth(unsigned width);
	unsigned getWidth() const;

	void setHeight(unsigned height);
	unsigned getHeight() const;

	void load(char* mapFilePath);
	void save(char* mapFilePath) const;

	Color getPixel(std::size_t x, std::size_t y) const;
	void setPixel(std::size_t x, std::size_t y, Color color);

	void setAround(Location location, Color color);
	void setAround(Location location, unsigned pixelSize, Color color);
	void setAround(unsigned x, unsigned y, Color color);
	void setAround(unsigned x, unsigned y, unsigned pixelSize, Color color);

	void setArea(unsigned top, unsigned left, unsigned bottom, unsigned right, Color color);

	void setLine(Location from, Location to, Color color);
	void setLine(Location from, Location to, unsigned pixelSize, Color color);
	void setLine(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY, Color color);
	void setLine(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY, unsigned pixelSize, Color color);

	Color& operator[](std::size_t i);
	const Color& operator[](std::size_t i) const;
private:
	void resize();
};

#endif /* IMAGE_H_ */
