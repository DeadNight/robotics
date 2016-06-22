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

void Solution::save(const char* mapFilePath) const {
	double resolutionRatio = (double)searchable.getGridResolution() / searchable.getMapResolution();
	unsigned pixelSize = resolutionRatio < 1 ? 1 : resolutionRatio;

	Image image = searchable.toImage();

	for(unsigned i = 1; i < path.size(); ++i) {
		Location from, to;
		from.setX(path[i - 1].getX() / searchable.getMapResolution());
		from.setY(path[i - 1].getY() / searchable.getMapResolution());
		to.setX(path[i].getX() / searchable.getMapResolution());
		to.setY(path[i].getY() / searchable.getMapResolution());

		image.setLine(from, to, pixelSize/2, Color::Red);

		if(i > 1)
			image.setAround(from, pixelSize, Color::Cyan);
	}

	Location start, goal;
	start.setX(searchable.getStartState().getX() / searchable.getMapResolution());
	start.setY(searchable.getStartState().getY() / searchable.getMapResolution());
	image.setAround(start, pixelSize, Color::Green);

	goal.setX(searchable.getGoalState().getX() / searchable.getMapResolution());
	goal.setY(searchable.getGoalState().getY() / searchable.getMapResolution());
	image.setAround(goal, pixelSize, Color::Blue);

	image.save(mapFilePath);
}

std::ostream& operator<<(std::ostream& out, const Solution& solution) {
	unsigned width = solution.searchable.getWidth();
	unsigned height = solution.searchable.getHeight();
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			unsigned pos = y*width + x;
			out << " ";
			if(y == solution.searchable.getStartState().getY() && x == solution.searchable.getStartState().getX()) {
				out << "S";
			} else if(y == solution.searchable.getGoalState().getY() && x == solution.searchable.getGoalState().getX()) {
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
