/*
 * Solution.h
 *
 *  Created on: May 7, 2016
 *      Author: user
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include "MapSearchable.h"
#include "Path.h"

class Solution {
	MapSearchable searchable;
	Path path;
public:
	Solution(MapSearchable searchable, Path path);

    MapSearchable getSearchable() const;
    void setSearchable(MapSearchable searchable);

    Path getPath() const;
    void setPath(Path path);

    void save(const char* mapFilePath, float mapResolution) const;

    friend std::ostream& operator<<(std::ostream& out, const Solution& solution);
};

#endif /* SOLUTION_H_ */
