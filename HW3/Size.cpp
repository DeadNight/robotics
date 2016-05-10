/*
 * Size.cpp
 *
 *  Created on: May 4, 2016
 *      Author: user
 */

#include "Size.h"

Size::Size(double width, double height)
{
	setWidth(width);
	setHeight(height);
}

ostream& operator<<(ostream& out, const Size& size) {
	return out << size.width << " " << size.height;
}

istream& operator>>(istream& in, Size& size) {
	return in >> size.width >> size.height;
}
