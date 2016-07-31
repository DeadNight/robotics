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
#include "Deltas.h"

using namespace std;
using namespace PlayerCc;

class Robot {
	static double obstacleMargin;
	static double maxSpeed;
	static double maxTurnSpeed;

	Size size;
	Position position;
	Path const* path;
	unsigned nextPathIndex;

	Deltas deltas;
	Position* lastPosition;

	PlayerClient *pc;
	Position2dProxy *pp;
	LaserProxy *lp;
	Graphics2dProxy *gp;
public:
	Robot(const char* host, unsigned port, Size size, Position position);
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
    Position getStagePosition(const Position& position) const;

    Location getStageLocation() const;
    Location getStageLocation(const Location& location) const;

    Position getWorldPosition() const;
    Location getWorldLocation(const Location& location) const;

    double getX() const;
    double getY() const;
    double getYaw() const;

    const LaserProxy& getLaserProxy() const;

    void setPath(const Path& p);

    bool isAt(const Location& l) const;

    void read();
    const Deltas& getDeltas() const;

    void enableMotor();
    void disableMotor();

    void move();

    void moveTo(const Path& path);
    void moveTo(const Location& location);

    bool hasObstacle() const;
	void avoidObstacle();

    double distanceTo(const Location& stageLocation) const;
    double angleTo(const Location& target) const;

    void drawPoint(const Position& stagePosition, Color color) const;
    void drawPoint(const Location& stageLocation, Color color) const;

    void drawPosition(const Position& position, Color color) const;
};

#endif /* ROBOT_H_ */
