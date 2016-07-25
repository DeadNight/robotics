/*
 * Path.h
 *
 *  Created on: May 4, 2016
 *      Author: user
 */

#ifndef PATH_H_
#define PATH_H_

#include <vector>
#include "Location.h"

class Path {
	std::vector<Location> path;
public:
	Path() { }
	Path(std::vector<Location> path);

	void setPath(std::vector<Location> path);

	unsigned size() const;

	void reduce();

    Location operator[](std::size_t i);
    Location operator[](std::size_t i) const;

	friend std::ostream& operator<<(std::ostream& out, const Path& path);
};

#endif /* PATH_H_ */
