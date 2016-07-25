/*
 * Partical.cpp
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#include "Particle.h"

Particle::Particle(const Map* map, const Position& p, double belief) : map(map), belief(belief) {
	setPosition(p);
}


double Particle::getBelief() const {
	return belief;
}

void Particle::setBelief(double belief) {
	this->belief = belief;
}

Position Particle::getPosition() const {
	std::cout << "*** " << position << " --> " << map->getWorldPosition(position) << std::endl;
	return map->getWorldPosition(position);
}

void Particle::setPosition(const Position& position) {
	this->position = map->getGridPosition(position);
}

void Particle::update(const LaserProxy& lp, const Deltas& deltas) {
	position.set(
		position.getX() + deltas.getX() / map->getGridResolution(),
		position.getY() + deltas.getY() / map->getGridResolution(),
		position.getYaw() + deltas.getYaw()
	);
	double newBelief = NORMAL * belief * probaByMove(deltas) * probaByLazer(lp);
	if(!isnan(newBelief))
		belief = newBelief;
}

void Particle::printParticle(const char* mapFilePath){
	Image image = map->toImage();
	Location l;
	l.setX(position.getLocation().getX() / map->getMapResolution());
	l.setY(position.getLocation().getY() / map->getMapResolution());
	image.setAround(l, Color::Red);
	image.save(mapFilePath);
}

void Particle::printParticle(Image& image){
	Location l;
	l.setX(position.getLocation().getX() / map->getMapResolution());
	l.setY(position.getLocation().getY() / map->getMapResolution());
	image.setAround(l, Color::Red);
}

Path Particle::linearPath(const Location& from, const Location& to){
	double x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	std::vector<Location> temp;
	double x1 = from.getX();
	double x2 = to.getX();
	double y1 = from.getY();
	double y2 = to.getY();

	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2*dy1 - dx1;
	py = 2*dx1 - dy1;

	if(dy1 <= dx1) {
		if(dx >= 0) {
			x = x1;
			y = y1;
			xe = x2;
		} else {
			x = x2;
			y = y2;
			xe = x1;
		}

		temp.push_back(Location(x, y));
		for(i = 0; x < xe; ++i) {
			x = x + 1;
			if(px < 0) {
				px = px + 2*dy1;
			} else {
				if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
					++y;
				} else {
					--y;
				}
				px += 2 * (dy1 - dx1);
			}

			temp.push_back(Location(x, y));
		}
	} else {
		if(dy >= 0) {
			x = x1;
			y = y1;
			ye = y2;
		} else {
			x = x2;
			y = y2;
			ye = y1;
		}

		temp.push_back(Location(x, y));

		for(i = 0; y < ye; ++i) {
			++y;
			if(py <= 0) {
				py += 2 * dx1;
			} else {
				if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
					++x;
				} else {
					--x;
				}
				py += 2 * (dx1 - dy1);
			}
			temp.push_back(Location(x, y));
		}
	}

	if (temp[0].getX() != from.getX() || temp[0].getY() != from.getY()) {
		std::reverse(temp.begin(),temp.end());
	}

	 return(Path(temp));
}

//Function that returns values between 0 to 1
//The higher the value of X, the lower the value that the function returns
double Particle::helpFunc (double x){
	return 1 - fabs(atan(x) / (PI/2));
}
double Particle::probaByLazer(const LaserProxy& lp){
	double hits = 0;
	int cnt = 0;
	//Image image = _map->toImage();

	for(uint i = 0; i < lp.GetCount(); ++i) {
		double d = 100 * lp[i];
		double angle = dtor(position.getYaw()) + lp.GetBearing(i);
		double xObs = position.getX() + d*cos(angle) / map->getGridResolution();
		double yObs = position.getY() - d*sin(angle) / map->getGridResolution();

		Path path = linearPath(Location(position.getX(), position.getY()), Location(xObs,yObs));

		/*for (unsigned z=0; z<temp.getPath().size();z++){
				image.setAround(Location(temp.getPath()[z].getX()/ _map->getMapResolution(),temp.getPath()[z].getY()/ _map->getMapResolution()),Color::Green);
		}
		image.setAround(Location(xObs/ _map->getMapResolution(),yObs/ _map->getMapResolution()),Color::Magenta);*/

		if(d < 100 * lp.GetMaxRange()){
			for(unsigned j = 0; j < path.size(); ++j) {
				if((*map)(path[j]) == 1) {
					if(fabs(path[j].getX() - xObs) < 1 && fabs(path[j].getY() - yObs) < 1) {
						++hits;
					} else if(fabs(path[j].getX()-xObs) < 2 && fabs(path[j].getY() - yObs < 2)) {
						hits += 0.9;
					}

/*					Location l(temp.getPath()[j].getX()/_map->getMapResolution(),temp.getPath()[j].getY()/ _map->getMapResolution());
					image.setAround(l,Color::Blue);*/
					break;
				}
			}
		} else {
			bool check = false;
			for(unsigned k = 0; k <= path.size(); ++k) {
				if((*map)(path[k]) == 1) {
					if(fabs(path[k].getX()) - xObs < 2 && fabs(path[k].getY() - yObs < 2)) {
						hits += 0.9;
					}
					check = true;
					/*Location l(temp.getPath()[k].getX()/_map->getMapResolution(),temp.getPath()[k].getY()/ _map->getMapResolution());
					image.setAround(l,Color::Blue);*/
					break;
				}
			}
			if(!check){
				++cnt;
				++hits;
				/*Location l(temp.getPath()[temp.getPath().size()-1].getX()/_map->getMapResolution(),temp.getPath()[temp.getPath().size()-1].getY()/ _map->getMapResolution());
				image.setAround(l,Color::Blue);*/
			}
		}
	}
	/*image.setAround(Location(this->getPosition().getX()/ _map->getMapResolution(),this->getPosition().getY()/ _map->getMapResolution()),Color::Red);
	image.save("test.png");*/
	return hits / lp.GetCount();
}

double Particle::probaByMove(const Deltas& deltas) {
	double disntance = sqrt(pow(deltas.getX(), 2) + pow(deltas.getY(), 2));
	return helpFunc(disntance + fabs(deltas.getYaw()));

 }

Position Particle::spawnPosition() {
	return Position(
			position.getX() + (rand() % (2*RANGE + 1)) - RANGE,
			position.getY() + (rand() % (2*RANGE + 1)) - RANGE,
			position.getYaw() + rand() % 90 - 45
	);
}

vector<Particle>::iterator Particle::find(vector<Particle>& list, const Particle& p) {
	for(vector<Particle>::iterator it = list.begin(); it != list.end(); ++it)
		if(p == *it)
			return it;
	return list.end();
}

vector <Particle> Particle::spawnParticles(int num) {
	vector<Particle> particles;

	srand(time(NULL));

	for(int i = 0; i < num; ++i) {
		Position pos = spawnPosition();

		if((*map)(pos.getLocation()) != 1) {
			Particle newParticle = Particle(map, map->getWorldPosition(pos), belief);

			if(particles.end() == find(particles, newParticle))
				particles.push_back(newParticle);
		}
	}

	return particles;
}
