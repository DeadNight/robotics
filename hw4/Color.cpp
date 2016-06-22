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

Color Color::Black(0, 0, 0);
Color Color::White(255, 255, 255);
Color Color::Gray(178, 178, 178);
Color Color::Red(255, 0, 0);
Color Color::Green(0, 255, 0);
Color Color::Blue(0, 0, 255);
Color Color::Yellow(255, 255, 0);
Color Color::Cyan(0, 255, 255);
Color Color::Magenta(255, 0, 255);

std::ostream& operator<<(std::ostream& out, const Color& color) {
	return out << (int)color.red << ", " << (int)color.green << ", " << (int)color.blue << ", " << (int)color.alpha;
}
