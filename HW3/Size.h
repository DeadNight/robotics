/*
 * Size.h
 *
 *  Created on: May 4, 2016
 *      Author: user
 */

#ifndef SIZE_H_
#define SIZE_H_

#include <iostream>

class Size {
	double width;
	double height;
public:
	Size() { }
	Size(double width, double height);

    double getWidth() const { return width; }
    void setWidth(double width) { this->width = width; }

    double getHeight() const { return height; }
    void setHeight(double height) { this->height = height; }

    friend std::ostream& operator<<(std::ostream& out, const Size& size);
    friend std::istream& operator>>(std::istream& in, Size& size);
};

#endif /* SIZE_H_ */
