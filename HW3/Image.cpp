/*
 * Image.cpp
 *
 *  Created on: May 7, 2016
 *      Author: user
 */

#include "Image.h"

Image::Image(unsigned width, unsigned height) : width(width), height(height) {
	resize();
}

Image::Image(Size size) : width(size.getWidth()), height(size.getHeight()) {
	resize();
}

void Image::setSize(Size size) {
	setWidth(size.getWidth());
	setHeight(size.getHeight());
}

Size Image::getSize() const {
	Size size(width, height);
	return size;
}

void Image::setWidth(unsigned width) {
	this->width = width;
	resize();
}

unsigned Image::getWidth() const {
	return width;
}

void Image::setHeight(unsigned height) {
	this->height = height;
	resize();
}

unsigned Image::getHeight() const {
	return height;
}

void Image::resize() {
	pixels.resize(width * height);
}

void Image::load(char* mapFilePath) {
	std::vector<unsigned char> rawPixels;

	//decode
	unsigned error = lodepng::decode(rawPixels, width, height, "/home/user/robotics/PcBotWorld/roboticLabMap.png");

	//if there's an error, display it
	if (error)
		std::cout << "decoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;

	pixels.resize(width * height);

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			unsigned pos = y*width*4 + x*4;
			Color color(rawPixels[pos], rawPixels[pos + 1], rawPixels[pos + 2], rawPixels[pos + 3]);
			setPixel(x, y, color);
		}
	}
}

void Image::save(char* mapFilePath) const {
	std::vector<unsigned char> rawPixels;
	unsigned rawSize = width * height * 4;

	rawPixels.resize(rawSize);

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			unsigned pos = y*width*4 + x*4;
			Color color = getPixel(x, y);
			rawPixels[pos + 0] = color.getRed();
			rawPixels[pos + 1] = color.getGreen();
			rawPixels[pos + 2] = color.getBlue();
			rawPixels[pos + 3] = color.getAlpha();
		}
	}

	//Encode the image
	unsigned error = lodepng::encode(mapFilePath, rawPixels, width, height);

	//if there's an error, display it
	if (error)
		std::cout << "encoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;
}

Color& Image::operator[](std::size_t i) {
	return pixels[i];
}

const Color& Image::operator[](std::size_t i) const {
	return pixels[i];
}

Color Image::getPixel(std::size_t x, std::size_t y) const {
	return pixels[y*width + x];
}

void Image::setPixel(std::size_t x, std::size_t y, Color color) {
	pixels[y*width + x] = color;
}

void Image::setAround(Location location, Color color) {
	setAround(location, 1, color);
}

void Image::setAround(Location location, unsigned pixelSize, Color color) {
	setAround(location.getX(), location.getY(), pixelSize, color);
}

void Image::setAround(unsigned x, unsigned y, Color color) {
	setAround(x, y, 1, color);
}

void Image::setAround(unsigned x, unsigned y, unsigned pixelSize, Color color) {
	unsigned halfSize = (double)pixelSize/2;
	unsigned top = y > halfSize ? y - halfSize : 0;
	unsigned left = x > halfSize ? x - halfSize : 0;
	unsigned bottom = y + halfSize < height ? y + halfSize : height;
	unsigned right = x + halfSize < width ? x + halfSize : width;
	setArea(top, left, bottom, right, color);
}

void Image::setArea(unsigned top, unsigned left, unsigned bottom, unsigned right, Color color) {
	for(unsigned y = top; y <= bottom; ++y)
		for(unsigned x = left; x <= right; ++x)
			setPixel(x, y, color);
}

void Image::setLine(Location from, Location to, Color color) {
	setLine(from, to, 1, color);
}

void Image::setLine(Location from, Location to, unsigned pixelSize, Color color) {
	setLine(from.getX(), from.getY(), to.getX(), to.getY(), pixelSize, color);
}

void Image::setLine(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY, Color color)
{
	setLine(fromX, fromY, toX, toY, 1, color);
}

void Image::setLine(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY, unsigned pixelSize, Color color) {
	unsigned top, bottom;
	if(fromY < toY) {
		bottom = fromY; top = toY;
	} else {
		bottom = toY; top = fromY;
	}

	if(toX == fromX) {
		unsigned x = toX;
		for(unsigned y = bottom; y <= top; ++y)
			setAround(x, y, pixelSize, color);
	} else {
		unsigned left, right;

		double m = ((double)toY - fromY)/((double)toX - fromX);
		double n = toY - m*toX;

		if(fromX < toX) {
			left = fromX; right = toX;
		} else {
			left = toX; right = fromX;
		}

		for(unsigned y = bottom; y <= top; ++y)
			for(unsigned x = left; x <= right; ++x)
				if(y == (unsigned)(m*x + n))
					setAround(x, y, pixelSize, color);
	}
}
