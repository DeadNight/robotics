/*
 * Robot.cpp
 *
 *  Created on: May 8, 2016
 *      Author: user
 */

#include "Robot.h"

double Robot::minMargin = 1.5;
double Robot::maxMargin = 2.75;
double Robot::maxSpeed = 0.5;
double Robot::maxTurnSpeed = 0.3;

Robot::Robot(const char* host, unsigned port, Size size, Position position, Map map) : size(size), map(map) {
	pc = new PlayerClient(host,port);
	pp = new Position2dProxy(pc);
	lp = new LaserProxy(pc);
	gp = new Graphics2dProxy(pc);

	setPosition(getStagePosition(position), true);
}

Robot::~Robot() {
	delete gp;
	delete lp;
	delete pp;
	delete pc;
}

Size Robot::getSize() const {
	return size;
}

void Robot::setSize(Size size) {
	this->size = size;
}

double Robot::getWidth() const {
	return size.getWidth();
}

void Robot::setWidth(double width) {
	size.setWidth(width);
}

double Robot::getHeight() const {
	return size.getHeight();
}

void Robot::setHeight(double height) {
	size.setHeight(height);
}

double Robot::getWidthMeters() const {
	return getWidth() / 100;
}

double Robot::getHeightMeters() const {
	return getHeight() / 100;
}

Position Robot::getPosition() const {
	return position;
}

void Robot::setPosition(Position position) {
	setPosition(position, false);
}

void Robot::setPosition(Position position, bool setOdometry) {
	this->position = position;
	if(setOdometry)
		pp->SetOdometry(position.getX(), position.getY(), position.getYaw());
}

Location Robot::getLocation() const {
	return position.getLocation();
}

Position Robot::getStagePosition() const {
	return getStagePosition(position);
}

Position Robot::getStagePosition(Position position) const {
	Position stagePosition(
		getStageLocation(position.getLocation()),
		dtor(position.getYaw())
	);
	return stagePosition;
}

Location Robot::getStageLocation() const {
	return getStageLocation(getLocation());
}

Location Robot::getStageLocation(Location location) const {
	Location stageLocation(
		((location.getX() / 100) * 2.5) - (13.75/2),
		-((location.getY() / 100) * 2.5) + (9.5/2)
	);
	return stageLocation;
}

double Robot::getX() const {
	return position.getX();
}

double Robot::getY() const {
	return position.getY();
}

double Robot::getYaw() const {
	return position.getYaw();
}

Map Robot::getMap() const {
	return map;
}

void Robot::setMap(Map map) {
	this->map = map;
}

void Robot::moveTo(Path path) {
	for(unsigned i = 1; i < path.size(); ++i)
		moveTo(path[i]);
}

void Robot::moveTo(Location location) {
	Location target = getStageLocation(location);

	player_color cyan;
	cyan.red = 0;
	cyan.green = 255;
	cyan.blue = 255;
	cyan.alpha = 255;

	double biggestSize = getWidth() > getHeight() ? getWidthMeters() : getHeightMeters();

	pp->SetMotorEnable(true);
	pp->SetSpeed(0, 0);
	double d;
	while((d = distanceTo(target)) > biggestSize/2) {
		pc->Read();
		gp->Clear();

		Position currentPosition(pp->GetXPos(), pp->GetYPos(), pp->GetYaw());
		setPosition(currentPosition);

		drawPoint(target, cyan);

		player_point_2d points[2];
		points[0].px = getX();
		points[0].py = getY();
		points[1].px = target.getX();
		points[1].py = target.getY();

		gp->Color(255, 0, 0, 255);
		gp->DrawPolyline(points, 2);


		double targetYaw = angleTo(target);

		double yawDiff = targetYaw - getYaw();

		while(yawDiff < dtor(180))
			yawDiff += dtor(360);
		while(yawDiff > dtor(180))
			yawDiff -= dtor(360);

		double speed = 0;
		if(abs(yawDiff) < 0.1)
			speed = maxSpeed;

		double turnSpeed = yawDiff;
		if(abs(turnSpeed) > maxTurnSpeed)
			turnSpeed = (turnSpeed/abs(turnSpeed))*maxTurnSpeed;

		pp->SetSpeed(speed, turnSpeed);
	}
	pp->SetSpeed(0, 0);
	pp->SetMotorEnable(false);
}

double Robot::distanceTo(Location target) const {
	return sqrt(pow(target.getX() - getX(), 2) + pow(target.getY() - getY(), 2));
}

double Robot::angleTo(Location target) const {
	double angle;
	if(target.getX() == getX()) {
		if(target.getY() > getY())
			angle = dtor(90);
		else
			angle = dtor(-90);
	} else {
		double m = (target.getY() - getY()) / (target.getX() - getX());
		angle = atan(m);
		if(target.getX() < getX())
			angle += dtor(180);
	}
	return angle;
}

void Robot::drawPoint(Location stageLocation, player_color color) const {
	player_point_2d points[4];
	player_point_2d point;
	point.px = stageLocation.getX();
	point.py = stageLocation.getY();

	points[0].px = point.px - 0.05;
	points[0].py = point.py - 0.05;

	points[1].px = point.px + 0.05;
	points[1].py = point.py - 0.05;

	points[2].px = point.px + 0.05;
	points[2].py = point.py + 0.05;

	points[3].px = point.px - 0.05;
	points[3].py = point.py + 0.05;

	gp->DrawPolygon(points, 4, true, color);
}
