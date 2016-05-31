/*
 * Color.cpp
 *
 *  Created on: May 7, 2016
 *      Author: user
 */

#include "Color.h"

Color::Color(unsigned char red, unsigned char green, unsigned char blue) {
	setRed(red);
	setGreen(green);
	setBlue(blue);
	setAlpha(255);
}

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
	setRed(red);
	setGreen(green);
	setBlue(blue);
	setAlpha(alpha);
}

unsigned char Color::getRed() const {
    return red;
}

void Color::setRed(unsigned char red) {
    this->red = red;
}

unsigned char Color::getGreen() const {
    return green;
}

void Color::setGreen(unsigned char green) {
    this->green = green;
}

unsigned char Color::getBlue() const {
    return blue;
}

void Color::setBlue(unsigned char blue) {
    this->blue = blue;
}

unsigned char Color::getAlpha() const {
    return alpha;
}

void Color::setAlpha(unsigned char alpha) {
    this->alpha = alpha;
}
