/*
 * Robot.cpp
 *
 *  Created on: May 8, 2016
 *      Author: user
 */

#include "Robot.h"

double Robot::obstacleMargin = 1.5;
double Robot::maxSpeed = 0.7/2;
double Robot::maxTurnSpeed = 0.5/2;

Robot::Robot(const char* host, unsigned port, Size size, Position position)
				: lastPosition(NULL), gp(NULL) {
	pc = new PlayerClient(host,port);
	pp = new Position2dProxy(pc);
	lp = new LaserProxy(pc);

	try {
		gp = new Graphics2dProxy(pc);
	} catch(...) {
		// ok, we're probably connected to the robot
	}

	setPosition(position);
	setSize(size);
	pp->SetOdometry(this->position.getX(), this->position.getY(), dtor(this->position.getYaw()));
}

Robot::~Robot() {
	if(gp != NULL)
		delete gp;
	delete lp;
	delete pp;
	delete pc;
	if(lastPosition != NULL)
		delete lastPosition;
}

Size Robot::getSize() const {
	return Size(size.getWidth()*100, size.getHeight()*100);
}

void Robot::setSize(Size size) {
	this->size.set(size.getWidth()/100, size.getHeight()/100);
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
	return getWorldPosition();
}

void Robot::setPosition(Position position) {
	this->position = getStagePosition(position);

	//pp->SetOdometry(getX(), getY(), dtor(getYaw()));

	drawPosition(this->position, Color::Magenta);
}

Location Robot::getLocation() const {
	return position.getLocation();
}

Position Robot::getStagePosition() const {
	return position;
}

Position Robot::getStagePosition(const Position& position) const {
	Position stagePosition(
		getStageLocation(position.getLocation()),
		position.getYaw()
	);
	return stagePosition;
}

Location Robot::getStageLocation() const {
	return getLocation();
}

Location Robot::getStageLocation(const Location& location) const {
	Location stageLocation(
		(location.getX() / 100) - (13.75/2),
		-(location.getY() / 100) + (9.5/2)
	);
	return stageLocation;
}

Position Robot::getWorldPosition() const {
	Position worldPosition(
		getWorldLocation(position.getLocation()),
		position.getYaw()
	);
	return worldPosition;
}

Location Robot::getWorldLocation(const Location& location) const {
	Location worldLocation(
		(location.getX() + (13.75/2)) * 100,
		-(location.getY() - (9.5/2))  * 100
	);
	return worldLocation;
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

void Robot::read() {
	pc->Read();

	if(lastPosition == NULL) {
		lastPosition = new Position(position);
	} else {
		deltas.set(100 * (pp->GetXPos() - lastPosition->getX()),
				   100 * (pp->GetYPos() - lastPosition->getY()),
				   rtod(pp->GetYaw()) - lastPosition->getYaw());

		lastPosition->set(pp->GetXPos(), pp->GetYPos(), rtod(pp->GetYaw()));

		position.set(position.getX() + deltas.getX()/100,
					 position.getY() - deltas.getY()/100,
					 position.getYaw() + deltas.getYaw());
	}
}

const Deltas& Robot::getDeltas() const {
	return deltas;
}

void Robot::enableMotor() {
	pp->SetSpeed(0, 0);
	pp->SetMotorEnable(true);
}

void Robot::disableMotor() {
	pp->SetMotorEnable(false);
}

void Robot::move() {
	while(nextPathIndex < path->size() && isAt((*path)[nextPathIndex])) {
		++nextPathIndex;
	}

	if(nextPathIndex < path->size()) {
		Location target = getStageLocation((*path)[nextPathIndex]);

		if(gp != NULL)
			gp->Clear();

		drawPosition(getStagePosition(position), Color::Magenta);
		drawPoint(target, Color::Cyan);

		player_point_2d points[2];
		points[0].px = getX();
		points[0].py = getY();
		points[1].px = target.getX();
		points[1].py = target.getY();

		if(gp != NULL) {
			gp->Color(255, 0, 0, 255);
			gp->DrawPolyline(points, 2);
		}

		double yaw = getYaw();
		while(yaw > 180) yaw -= 360;
		while(yaw < -180) yaw += 360;

		double targetYaw = angleTo(target);
		while(yaw > 180) yaw -= 360;
		while(yaw < -180) yaw += 360;

		double yawDiff = targetYaw - yaw;

		while(yawDiff < 180)
			yawDiff += 360;
		while(yawDiff > 180)
			yawDiff -= 360;

		double speed;
		if(abs(yawDiff) < 5)
			speed = maxSpeed;
		else if(abs(yawDiff) > 45)
			speed = 0;
		else
			speed = maxSpeed * (1 - (abs(yawDiff) / 45));

		double turnSpeed = dtor(yawDiff);
		if(abs(turnSpeed) > maxTurnSpeed)
			turnSpeed = (turnSpeed/abs(turnSpeed))*maxTurnSpeed;

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
		if(gp != NULL)
			gp->Clear();

		Position currentPosition(pp->GetXPos(), pp->GetYPos(), pp->GetYaw());
		setPosition(currentPosition);

		drawPoint(target, Color::Cyan);

		player_point_2d points[2];
		points[0].px = getX();
		points[0].py = getY();
		points[1].px = target.getX();
		points[1].py = target.getY();

		if(gp != NULL) {
			gp->Color(255, 0, 0, 255);
			gp->DrawPolyline(points, 2);
		}

		double yaw = getYaw();
		while(yaw > 180) yaw -= 360;
		while(yaw < -180) yaw += 360;

		double targetYaw = angleTo(target);
		while(yaw > 180) yaw -= 360;
		while(yaw < -180) yaw += 360;

		double yawDiff = targetYaw - yaw;

		while(yawDiff < 180)
			yawDiff += 360;
		while(yawDiff > 180)
			yawDiff -= 360;

		double speed;
		if(abs(yawDiff) < 5)
			speed = maxSpeed;
		else if(abs(yawDiff) > 45)
			speed = 0;
		else
			speed = maxSpeed * (1 - (abs(yawDiff) / 45));

		double turnSpeed = dtor(yawDiff);
		if(abs(turnSpeed) > maxTurnSpeed)
			turnSpeed = (turnSpeed/abs(turnSpeed))*maxTurnSpeed;

		pp->SetSpeed(speed, turnSpeed);
	}
	pp->SetSpeed(0, 0);
	pp->SetMotorEnable(false);
}

bool Robot::hasObstacle() const {
	double maxAngle = atan(size.getWidth()/size.getHeight());
	double angleStep = 2*lp->GetMaxAngle()/lp->GetCount();
	int minIndex = (lp->GetMaxAngle() - maxAngle) / angleStep;
	for(unsigned i = minIndex; i < lp->GetCount() - minIndex; ++i) {
		double d = (*lp)[i];
		double angle = lp->GetBearing(i);

		if(abs(d*cos(angle)) < obstacleMargin*size.getHeight() && abs(d*sin(angle)) < obstacleMargin*size.getWidth()) {
			return true;
		}
	}
	return false;
}

void Robot::avoidObstacle() {
	double turnDirection;

	if(deltas.getYaw() == 0) {
		double weight = 0, maxWeight = 0;
		int mid = lp->GetCount()/2;

		if(gp != NULL)
			gp->Clear();

		for(unsigned i = 0; i < lp->GetCount(); ++i) {
			if(gp != NULL) {
				double d = (*lp)[i];
				double angle = lp->GetBearing(i);

				if(abs(d*cos(angle)) < obstacleMargin*size.getHeight() && abs(d*sin(angle)) < obstacleMargin*size.getWidth()) {
					player_point_2d points[2];
					points[0].px = pp->GetXPos();
					points[0].py = pp->GetYPos();

					points[1].px = pp->GetXPos() + d*cos(pp->GetYaw() + angle);
					points[1].py = pp->GetYPos() + d*sin(pp->GetYaw() + angle);

					gp->Color(255, 0, 0, 255);
					gp->DrawPolyline(points, 2);
				}
			}

			if((*lp)[mid - i] >= lp->GetMaxRange()) {
				--maxWeight;
			}
			if((*lp)[mid + i] >= lp->GetMaxRange()) {
				++maxWeight;
			}
			weight -= (*lp)[mid - i];
			weight += (*lp)[mid + i];
		}


		if(maxWeight > 0) {
			turnDirection = 1;
		} else if(maxWeight < 0) {
			turnDirection = -1;
		} else if(weight > 0) {
			turnDirection = 1;
		} else {
			turnDirection = -1;
		}
	} else {
		turnDirection = deltas.getYaw()/fabs(deltas.getYaw());
	}

	pp->SetSpeed(0, turnDirection*maxTurnSpeed);
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

	if(gp != NULL) {
		gp->Color(playerColor);
		gp->DrawPolygon(points, 4, true, playerColor);
	}
}

void Robot::drawPosition(const Position& position, Color color) const {
	double angle = dtor(position.getYaw());
	player_point_2d points[5];
	player_point_2d point;

	point.px = position.getX();
	point.py = position.getY();

	player_color playerColor;
	playerColor.red = color.getRed();
	playerColor.green = color.getGreen();
	playerColor.blue = color.getBlue();
	playerColor.alpha = color.getAlpha();

	points[0].px = point.px + 0.1*cos(angle - dtor(180));
	points[0].py = point.py + 0.1*sin(angle - dtor(180));

	points[1].px = point.px;
	points[1].py = point.py;

	points[2].px = point.px + 0.05*cos(angle - dtor(135));
	points[2].py = point.py + 0.05*sin(angle - dtor(135));

	points[3].px = point.px + 0.05*cos(angle + dtor(135));
	points[3].py = point.py + 0.05*sin(angle + dtor(135));

	points[4].px = point.px;
	points[4].py = point.py;

	if(gp != NULL) {
		gp->Color(playerColor);
		gp->DrawPolygon(points, 5, true, playerColor);
	}
}
