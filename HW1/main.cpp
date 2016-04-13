/*
 * main1.cpp
 *
 *  Created on: Mar 29, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */


#include <iostream>
#include <math.h>
#include <libplayerc++/playerc++.h>

using namespace std;
using namespace PlayerCc;

const float robotWidth = 0.4;
const float robotDepth = 0.5;
const double maxAngle = atan(robotWidth/robotDepth);

const float minMargin = 1.5;
const float maxMargin = 2.75;

const float xSpeed = 0.5;
const float yawSpeed = 0.3;

int main(int argc, char** argv) {
	PlayerClient pc("localhost",6665);
	Position2dProxy pp(&pc);
	LaserProxy lp(&pc);
	Graphics2dProxy gp(&pc);

	pp.SetOdometry(-6.009, 2.481, dtor(-194.220));
	pp.SetMotorEnable(true);

	int turnDirection = 0;

	while(true) {
		pc.Read();
		gp.Clear();

		// print all obstacles in range

		double x = pp.GetXPos();
		double y = pp.GetYPos();
		double yaw = pp.GetYaw();

		player_point_2d points[lp.GetCount()];
		int empty[lp.GetCount()];
		int count = 0;
		uint countEmpty = 0;

		empty[countEmpty++] = 0;

		for(uint i = 0; i < lp.GetCount(); ++i) {
			if(lp[i] < lp.GetMaxRange()) {
				double d = lp[i];
				double angle = yaw + lp.GetBearing(i);


				double xObs = x + d*cos(angle);
				double yObs = y + d*sin(angle);

				player_point_2d point[1] = {{xObs, yObs}};
				points[count++] = point[0];
			} else {
				if(count == 0 || lp[i - 1] == lp.GetMaxRange()) {
					continue;
				} else {
					empty[countEmpty++] = count;
				}
			}
		}

		if(lp[count] < lp.GetMaxRange())
			empty[countEmpty++] = count - 1;

		cout<<endl<<"There are "<<countEmpty - 1<<" obstacles around me:"<<endl;
		if(count > 0) {
			for(uint i = 1; i < countEmpty; ++i) {
				cout<<i<<") from "<<points[empty[i - 1]]<<" to "<<points[empty[i]]<<endl;
			}
		}

		// check if there are obstacles ahead that the robot can't go through

		double angleStep = 2*lp.GetMaxAngle()/lp.GetCount();
		int minIndex = (lp.GetMaxAngle() - maxAngle) / angleStep;
		bool obstacleInFront = false;
		bool frontObstacleClose = false;
		for(int i = minIndex; i < lp.GetCount() - minIndex; ++i) {
			double d = lp[i];
			double angle = lp.GetBearing(i);

			double minDistance = MIN(lp.GetMaxRange(), abs((robotWidth/2) / sin(angle)));
			if(d < minDistance) {
				player_point_2d points[2] = {
					{pp.GetXPos(), pp.GetYPos()},
					{pp.GetXPos() + d*cos(pp.GetYaw() + angle), pp.GetYPos() + d*sin(pp.GetYaw() + angle)}
				};

				double xDistance = abs(d*cos(angle));

				if(xDistance < minMargin*robotDepth) {
					obstacleInFront = true;
					frontObstacleClose = true;
					gp.Color(255, 0, 0, 255);
				} else if(xDistance < maxMargin*robotDepth) {
					obstacleInFront = true;
					gp.Color(255, 165, 0, 255);
				} else {
					gp.Color(0, 255, 0, 255);
				}

				gp.DrawPolyline(points, 2);

			}
		}

		if(obstacleInFront) {
			if(!turnDirection) {
				double weight = 0, maxWeight = 0;
				int mid = lp.GetCount()/2;

				for(int i = 1; i < mid; ++i) {
					if(lp[mid - i] >= lp.GetMaxRange()) {
						--maxWeight;
					}
					if(lp[mid + i] >= lp.GetMaxRange()) {
						++maxWeight;
					}
					weight -= lp[mid - i];
					weight += lp[mid + i];
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

				cout<<"there is an obstacle in front of me... turning ";
				if(turnDirection > 0) {
					cout<<"left";
				} else {
					cout<<"right";
				}
				cout<<endl;

			}

			if(frontObstacleClose) {
				pp.SetSpeed(0, turnDirection*yawSpeed);
			} else {
				pp.SetSpeed(xSpeed/2, turnDirection*yawSpeed);
			}
		} else {
			if(turnDirection) {
				cout<<"all clear, moving forward"<<endl;
			}
			turnDirection = 0;
			pp.SetSpeed(xSpeed, 0);
		}
	}

	return 0;
}
