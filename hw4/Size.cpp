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

std::ostream& operator<<(std::ostream& out, const Size& size) {
	return out << size.width << " " << size.height;
}

std::istream& operator>>(std::istream& in, Size& size) {
	return in >> size.width >> size.height;
}
