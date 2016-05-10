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

Robot::Robot(char* host, unsigned port, Size size, Position position, Map map) : size(size), map(map) {
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
	getStageLocation(getLocation());
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

		double m = (target.getY() - getY()) / (target.getX() - getX());
		double targetYaw = atan(m);

		double yawDiff = targetYaw - getYaw();

		double turnSpeed = dtor(180) - abs(yawDiff);

		cout << turnSpeed << endl;

		if(abs(turnSpeed) > maxTurnSpeed) {
			turnSpeed = (turnSpeed/abs(turnSpeed))*maxTurnSpeed;
			cout << "capped " << turnSpeed << endl;
		}

		pp->SetSpeed(0, turnSpeed);
	}
	pp->SetMotorEnable(false);
}

double Robot::distanceTo(Location stageLocation) const {
	return sqrt(pow(pp->GetXPos() - stageLocation.getX(), 2) + (pp->GetYPos() - stageLocation.getY(), 2));
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

void Robot::roam() {
	pp->SetMotorEnable(true);

	int turnDirection = 0;

	while(true) {
		pc->Read();
		gp->Clear();

		// print all obstacles in range

		double x = pp->GetXPos();
		double y = pp->GetYPos();
		double yaw = pp->GetYaw();

		player_point_2d points[lp->GetCount()];
		int empty[lp->GetCount()];
		int count = 0;
		uint countEmpty = 0;

		empty[countEmpty++] = 0;

		for(uint i = 0; i < lp->GetCount(); ++i) {
			if((*lp)[i] < lp->GetMaxRange()) {
				double d = (*lp)[i];
				double angle = yaw + lp->GetBearing(i);


				double xObs = x + d*cos(angle);
				double yObs = y + d*sin(angle);

				player_point_2d point[1] = {{xObs, yObs}};
				points[count++] = point[0];
			} else {
				if(count == 0 || (*lp)[i - 1] == lp->GetMaxRange()) {
					continue;
				} else {
					empty[countEmpty++] = count;
				}
			}
		}

		if((*lp)[count] < lp->GetMaxRange())
			empty[countEmpty++] = count - 1;

		// check if there are obstacles ahead that the robot can't go through

		double maxAngle = atan((getWidthMeters())/(getHeight()/100));
		double angleStep = 2*lp->GetMaxAngle()/lp->GetCount();
		int minIndex = (lp->GetMaxAngle() - maxAngle) / angleStep;
		bool obstacleInFront = false;
		bool frontObstacleClose = false;
		for(int i = minIndex; i < lp->GetCount() - minIndex; ++i) {
			double d = (*lp)[i];
			double angle = lp->GetBearing(i);

			double minDistance = MIN(lp->GetMaxRange(), abs(((getWidthMeters())/2) / sin(angle)));
			if(d < minDistance) {
				player_point_2d points[2] = {
					{pp->GetXPos(), pp->GetYPos()},
					{pp->GetXPos() + d*cos(pp->GetYaw() + angle), pp->GetYPos() + d*sin(pp->GetYaw() + angle)}
				};

				double xDistance = abs(d*cos(angle));

				if(xDistance < minMargin*getHeightMeters()) {
					obstacleInFront = true;
					frontObstacleClose = true;
					gp->Color(255, 0, 0, 255);
				} else if(xDistance < maxMargin*getHeightMeters()) {
					obstacleInFront = true;
					gp->Color(255, 165, 0, 255);
				} else {
					gp->Color(0, 255, 0, 255);
				}

				gp->DrawPolyline(points, 2);

			}
		}

		if(obstacleInFront) {
			if(!turnDirection) {
				double weight = 0, maxWeight = 0;
				int mid = lp->GetCount()/2;

				for(int i = 1; i < mid; ++i) {
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
			}

			if(frontObstacleClose) {
				pp->SetSpeed(0, turnDirection*maxTurnSpeed);
			} else {
				pp->SetSpeed(maxSpeed/2, turnDirection*maxTurnSpeed);
			}
		} else {
			turnDirection = 0;
			pp->SetSpeed(maxSpeed, 0);
		}
	}
}
