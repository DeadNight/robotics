/*
 * Robot.h
 *
 *  Created on: May 8, 2016
 *      Author: user
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <libplayerc++/playerc++.h>
#include <cmath>
#include "Size.h"
#include "Position.h"
#include "Map.h"
#include "Location.h"
#include "Path.h"

using namespace std;
using namespace PlayerCc;

class Robot {
	static double minMargin;
	static double maxMargin;
	static double maxSpeed;
	static double maxTurnSpeed;

	PlayerClient *pc;
	Position2dProxy *pp;
	LaserProxy *lp;
	Graphics2dProxy *gp;

	Size size;
	Position position;
	Map map;
public:
	Robot(const char* host, unsigned port, Size size, Position position, Map map);
	virtual ~Robot();

	Size getSize() const;
	void setSize(Size size);

	double getWidth() const;
	void setWidth(double width);

	double getHeight() const;
	void setHeight(double height);

	double getWidthMeters() const;
	double getHeightMeters() const;

	Position getPosition() const;
    void setPosition(Position position);
    void setPosition(Position position, bool setOdometry);

    Location getLocation() const;

    Position getStagePosition() const;
    Position getStagePosition(Position position) const;

    Location getStageLocation() const;
    Location getStageLocation(Location location) const;

    double getX() const;
    double getY() const;
    double getYaw() const;

    Map getMap() const;
    void setMap(Map map);

    void moveTo(Path path);
    void moveTo(Location location);

    double distanceTo(Location stageLocation) const;
    double angleTo(Location target) const;

    void drawPoint(Location stageLocation, player_color color) const;
};

#endif /* ROBOT_H_ */
