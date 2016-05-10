/*
 * Size.h
 *
 *  Created on: May 4, 2016
 *      Author: user
 */

#ifndef SIZE_H_
#define SIZE_H_

#include <iostream.h>

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

    friend ostream& operator<<(ostream& out, const Size& size);
    friend istream& operator>>(istream& in, Size& size);
};

#endif /* SIZE_H_ */
