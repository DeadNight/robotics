/*
 * Color.h
 *
 *  Created on: May 7, 2016
 *      Author: user
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <iostream>

class Color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
public:
	Color() { } ;
	Color(unsigned char red, unsigned char green, unsigned char blue);
	Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

    unsigned char getRed() const;
    void setRed(unsigned char red);

    unsigned char getGreen() const;
    void setGreen(unsigned char green);

    unsigned char getBlue() const;
    void setBlue(unsigned char blue);

    unsigned char getAlpha() const;
    void setAlpha(unsigned char alpha);

    static Color Black;
    static Color White;
    static Color Gray;
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Yellow;
	static Color Cyan;
	static Color Magenta;

	friend std::ostream& operator<<(std::ostream& out, const Color& color);
};

#endif /* COLOR_H_ */
