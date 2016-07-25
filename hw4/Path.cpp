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

void Path::setPath(std::vector<Location> path) {
	this->path = path;
}

unsigned Path::size() const {
	return path.size();
}

void Path::reduce() {
	if(size() > 2) {
		std::vector<Location> reducedPath;
		std::vector<Location>::iterator it = path.begin();

		reducedPath.push_back(*it);
		Location lastLocation = path[0];
		double lastAngle = path[0].angleTo(path[1]);

		while(++it != path.end()) {
			double angle = lastLocation.angleTo(*it);
			lastLocation = *it;
			if(angle != lastAngle) {
				reducedPath.push_back(*it);
				lastAngle = angle;
			}
		}

		reducedPath.push_back(path[path.size() - 1]);
		path.swap(reducedPath);
	}
}

Location Path::operator[](std::size_t i) {
	return path[i];
}

Location Path::operator[](std::size_t i) const {
	return path[i];
}

std::ostream& operator<<(std::ostream& out, const Path& path) {
	for(unsigned i = 0; i < path.path.size(); ++i) {
		if(i > 0)
			out << " -> ";
		out << "(" << path.path[i] << ")";
	}
	return out << std::endl;
}
