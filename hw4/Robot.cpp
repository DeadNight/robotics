/*
 * Robot.cpp
 *
 *  Created on: May 8, 2016
 *      Author: user
 */

#include "Robot.h"

double Robot::minMargin = 1.5;
double Robot::maxMargin = 2.75;
double Robot::maxSpeed = 0.7;
double Robot::maxTurnSpeed = 0.5;

Robot::Robot(const char* host, unsigned port, Size size, Position position)
				: size(size), lastPosition(NULL) {
	pc = new PlayerClient(host,port);
	pp = new Position2dProxy(pc);
	lp = new LaserProxy(pc);
	gp = new Graphics2dProxy(pc);

	setPosition(position);
	pp->SetOdometry(position.getX(), position.getY(), position.getYaw());
}

Robot::~Robot() {
	delete gp;
	delete lp;
	delete pp;
	delete pc;
	if(lastPosition != NULL)
		delete lastPosition;
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
	this->position = getStagePosition(position);

	drawPoint(position, Color::Magenta);
}

Location Robot::getLocation() const {
	return position.getLocation();
}

Position Robot::getStagePosition() const {
	return getStagePosition(position);
}

Position Robot::getStagePosition(const Position& position) const {
	Position stagePosition(
		getStageLocation(position.getLocation()),
		dtor(position.getYaw())
	);
	return stagePosition;
}

Location Robot::getStageLocation() const {
	return getStageLocation(getLocation());
}

Location Robot::getStageLocation(const Location& location) const {
	Location stageLocation(
		(location.getX() / 100) - (13.75/2),
		-(location.getY() / 100) + (9.5/2)
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

const LaserProxy& Robot::getLaserProxy() const {
	return *lp;
}

void Robot::setPath(const Path& p) {
	path = &p;

	double d, minDist = distanceTo((*path)[1]);

	for(unsigned i = 0; i < path->size(); ++i) {
		if((d = distanceTo(getStageLocation((*path)[i]))) <= minDist) {
			minDist = d;
			nextPathIndex = i + 1;
		}
	}
}

bool Robot::isAt(const Location& l) const {
	double biggestSize = getWidth() > getHeight() ? getWidthMeters() : getHeightMeters();
	return distanceTo(getStageLocation(l)) <= biggestSize/2;
}

Deltas Robot::read() {
	pc->Read();

	if(lastPosition == NULL) {
		lastPosition = new Position(pp->GetXPos(), pp->GetYPos(), pp->GetYaw());
		return Deltas(0, 0, 0);
	} else {
		Deltas deltas(100 * (pp->GetXPos() - lastPosition->getX())
							, 100 * (pp->GetYPos() - lastPosition->getY())
							, pp->GetYaw() - lastPosition->getYaw());
		lastPosition->set(pp->GetXPos(), pp->GetYPos(), pp->GetYaw());
		return deltas;
	}
}

void Robot::move() {
	while(nextPathIndex < path->size() && isAt((*path)[nextPathIndex])) {
		++nextPathIndex;
	}

	//std::cout << "*** next: " << nextPathIndex << std::endl;

	if(nextPathIndex < path->size()) {
		Location target = getStageLocation((*path)[nextPathIndex]);

		gp->Clear();

		drawPoint(position.getLocation(), Color::Magenta);
		drawPoint(target, Color::Cyan);

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

		double speed;
		if(abs(yawDiff) < 0.1)
			speed = maxSpeed;
		else if(abs(yawDiff) > dtor(45))
			speed = 0;
		else
			speed = maxSpeed * (1 - (abs(yawDiff) / dtor(45)));

		if(speed > maxSpeed)
			speed = maxSpeed;

		double turnSpeed = yawDiff;
		if(abs(turnSpeed) > maxTurnSpeed)
			turnSpeed = (turnSpeed/abs(turnSpeed))*maxTurnSpeed;

		pp->SetMotorEnable(true);
		pp->SetSpeed(speed, turnSpeed);
	} else {
		pp->SetSpeed(0, 0);
		pp->SetMotorEnable(false);
	}
}

void Robot::moveTo(const Path& path) {
	for(unsigned i = 1; i < path.size(); ++i)
		moveTo(path[i]);
}

void Robot::moveTo(const Location& location) {
	Location target = getStageLocation(location);

	double biggestSize = getWidth() > getHeight() ? getWidthMeters() : getHeightMeters();

	pp->SetMotorEnable(true);
	pp->SetSpeed(0, 0);
	double d;
	while((d = distanceTo(target)) > biggestSize/2) {
		pc->Read();
		gp->Clear();

		Position currentPosition(pp->GetXPos(), pp->GetYPos(), pp->GetYaw());
		setPosition(currentPosition);

		drawPoint(target, Color::Cyan);

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

		double speed;
		if(abs(yawDiff) < 0.1)
			speed = maxSpeed;
		else if(abs(yawDiff) > dtor(45))
			speed = 0;
		else
			speed = maxSpeed * (1 - (abs(yawDiff) / dtor(45)));

		if(speed > maxSpeed)
			speed = maxSpeed;

		double turnSpeed = yawDiff;
		if(abs(turnSpeed) > maxTurnSpeed)
			turnSpeed = (turnSpeed/abs(turnSpeed))*maxTurnSpeed;

		pp->SetSpeed(speed, turnSpeed);
	}
	pp->SetSpeed(0, 0);
	pp->SetMotorEnable(false);
}

double Robot::distanceTo(const Location& target) const {
	return getLocation().distanceTo(target);
}

double Robot::angleTo(const Location& target) const {
	return getLocation().angleTo(target);
}

void Robot::drawPoint(const Position& stagePosition, Color color) const {
	drawPoint(stagePosition.getLocation(), color);
}

void Robot::drawPoint(const Location& stageLocation, Color color) const {
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

	player_color playerColor;
	playerColor.red = color.getRed();
	playerColor.green = color.getGreen();
	playerColor.blue = color.getBlue();
	playerColor.alpha = color.getAlpha();

	gp->DrawPolygon(points, 4, true, playerColor);
}
