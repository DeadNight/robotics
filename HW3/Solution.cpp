/*
 * Solution.cpp
 *
 *  Created on: May 7, 2016
 *      Author: user
 */

#include "Solution.h"

Solution::Solution(MapSearchable searchable, Path path) {
	setSearchable(searchable);
	setPath(path);
}

MapSearchable Solution::getSearchable() const {
    return searchable;
}

void Solution::setSearchable(MapSearchable searchable){
    this->searchable = searchable;
}

Path Solution::getPath() const {
    return path;
}

void Solution::setPath(Path path) {
    this->path = path;
}

void Solution::save(const char* mapFilePath, float mapResolution) const {
	double resolutionRatio = (double)searchable.getMap().getGridResolution() / mapResolution;
	unsigned pixelSize = resolutionRatio < 1 ? 1 : resolutionRatio;

	Color red(255, 0, 0);
	Color green(0, 255, 0);
	Color blue(0, 0, 255);
	Color cyan(0, 255, 255);

	Image image = searchable.toImage(mapResolution);

	for(unsigned i = 1; i < path.size(); ++i) {
		Location from, to;
		from.setX(path[i - 1].getX());
		from.setY(path[i - 1].getY());
		to.setX(path[i].getX());
		to.setY(path[i].getY());

		image.setLine(from, to, pixelSize, red);

		if(i > 1)
			image.setAround(from, pixelSize, cyan);
	}

	Location start, goal;
	start.setX(searchable.getStart().getX()*resolutionRatio);
	start.setY(searchable.getStart().getY()*resolutionRatio);
	image.setAround(start, pixelSize, green);

	goal.setX(searchable.getGoal().getX()*resolutionRatio);
	goal.setY(searchable.getGoal().getY()*resolutionRatio);
	image.setAround(goal, pixelSize, blue);

	image.save(mapFilePath);
}

std::ostream& operator<<(std::ostream& out, const Solution& solution) {
	unsigned width = solution.searchable.getWidth();
	unsigned height = solution.searchable.getHeight();
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			unsigned pos = y*width + x;
			out << " ";
			if(y == solution.searchable.getStart().getLocation().getY() && x == solution.searchable.getStart().getLocation().getX()) {
				out << "S";
			} else if(y == solution.searchable.getGoal().getY() && x == solution.searchable.getGoal().getX()) {
				out << "G";
			} else if(solution.searchable[pos] == 255)
				out << "@";
			else if (solution.searchable[pos] > 200)
				out << "#";
			else if (solution.searchable[pos] > 150)
				out << "%";
			else if (solution.searchable[pos] > 100)
				out << "o";
			else if (solution.searchable[pos] > 50)
				out << "-";
			else if (solution.searchable[pos] > 0)
				out << ".";
			else // grid[pos] == 0
				out << " ";
		}
		out << std::endl;
	}
	return out;
}
