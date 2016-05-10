/*
 * Path.cpp
 *
 *  Created on: May 4, 2016
 *      Author: user
 */

#include "Path.h"

Path::Path(std::vector<Location> path) {
	setPath(path);
}

std::vector<Location> Path::getPath() const {
	return path;
}

void Path::setPath(std::vector<Location> path) {
	this->path = path;
}

unsigned Path::size() const {
	return path.size();
}

Location Path::operator[](std::size_t i) {
	return path[i];
}

Location Path::operator[](std::size_t i) const {
	return path[i];
}

ostream& operator<<(ostream& out, const Path& path) {
	for(unsigned i = 0; i < path.path.size(); ++i) {
		if(i > 0)
			out << " -> ";
		out << "(" << path.path[i] << ")";
	}
	return out << endl;
}
