/*
 * Partical.cpp
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#include "Particle.h"

Particle::Particle(const Map& map, const Position& p, double belief) : belief(belief) {
	Position pos(map.getWorldLocation(map.getGridPosition(p).getLocation())
					, p.getYaw());
	setPosition(pos);
}


double Particle::getBelief() const {
	return belief;
}

void Particle::setBelief(double belief) {
	this->belief = belief;
}

const Position& Particle::getPosition() const {
	return position;
}

void Particle::setPosition(const Position& position) {
	this->position = position;
}

void Particle::update(const LaserProxy& lp, const Deltas& deltas, const Map& map) {
	position.set(position.getX() + deltas.getX()
					, position.getY() + deltas.getY()
					, position.getYaw() + deltas.getYaw());
	double newBelief = NORMAL * belief * probaByMove(deltas) * probaByLazer(lp, map);
	if(!isnan(newBelief))
		belief = newBelief;
}

void Particle::printParticle(const Map& map, const char* mapFilePath){
	Image image = map.toImage();
	Location l;
	l.setX(position.getLocation().getX() / map.getMapResolution());
	l.setY(position.getLocation().getY() / map.getMapResolution());
	image.setAround(l, Color::Red);
	image.save(mapFilePath);
}

void Particle::printParticle(const Map& map, Image& image){
	Location l;
	l.setX(position.getLocation().getX() / map.getMapResolution());
	l.setY(position.getLocation().getY() / map.getMapResolution());
	image.setAround(l, Color::Red);
}
Path Particle::linearPath(const Location& from, const Location& to){
	double x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
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
			x=x2;
			y=y2;
			xe=x1;
		}

		temp.push_back(Location(x,y));
		for(i=0;x<xe;i++) {
			x = x + 1;
			if(px < 0) {
				px = px + 2*dy1;
			} else {
				if((dx<0 && dy<0) || (dx>0 && dy>0)) {
					y=y+1;
				} else {
					y=y-1;
				}
				px=px+2*(dy1-dx1);
			}

			temp.push_back(Location(x,y));
		}
	} else {
		if(dy>=0) {
			x=x1;
			y=y1;
			ye=y2;
		} else {
			x=x2;
			y=y2;
			ye=y1;
		}

		temp.push_back(Location(x,y));

		for(i=0;y<ye;i++) {
			y=y+1;
			if(py<=0) {
				py=py+2*dx1;
			} else {
				if((dx<0 && dy<0) || (dx>0 && dy>0)) {
					x=x+1;
				} else {
					x=x-1;
				}
				py=py+2*(dx1-dy1);
			}
			temp.push_back(Location(x,y));
		}
	}

	if (temp[0].getX()!=from.getX() || temp[0].getY()!=from.getY()) {
		std::reverse(temp.begin(),temp.end());
	}

	 return(Path(temp));
}

//Function that returns values between 0 to 1
//The higher the value of X, the lower the value that the function returns
double Particle::helpFunc (double x){
	return 1 - fabs(atan(x) / (PI/2));
}
double Particle::probaByLazer(const LaserProxy& lp, const Map& map){
	double hits = 0;
	int cnt = 0;
	//Image image = _map->toImage();

	for(uint i = 0; i < lp.GetCount(); ++i) {
		double d = 100 * lp[i];
		double angle = dtor(position.getYaw()) + lp.GetBearing(i);
		double xObs = position.getX() + d*cos(angle);
		double yObs = position.getY() - d*sin(angle);

		Path temp = linearPath(Location(position.getX(), position.getY()), Location(xObs,yObs));

		/*for (unsigned z=0; z<temp.getPath().size();z++){
				image.setAround(Location(temp.getPath()[z].getX()/ _map->getMapResolution(),temp.getPath()[z].getY()/ _map->getMapResolution()),Color::Green);
		}
		image.setAround(Location(xObs/ _map->getMapResolution(),yObs/ _map->getMapResolution()),Color::Magenta);*/

		if(d < 100 * lp.GetMaxRange()){
			for(unsigned j =0 ; j < temp.getPath().size(); ++j){
				Location location = map.getGridLocation(Location(temp.getPath()[j].getX(), temp.getPath()[j].getY()));
				if (map(location) == 1){
					if(fabs(temp.getPath()[j].getX() - xObs) < 1 * map.getGridResolution() && fabs(temp.getPath()[j].getY() - yObs) < 1 * map.getGridResolution()) {
						++hits;
					} else if (fabs(temp.getPath()[j].getX()-xObs)<2 * map.getGridResolution() && fabs(temp.getPath()[j].getY()-yObs<2 * map.getGridResolution())) {
						hits += 0.9;
					}

/*					Location l(temp.getPath()[j].getX()/_map->getMapResolution(),temp.getPath()[j].getY()/ _map->getMapResolution());
					image.setAround(l,Color::Blue);*/
					break;
				}
			}
		} else {
			bool check = false;
			for(unsigned k = 0; k <= temp.getPath().size(); k++){
				Location location = map.getGridLocation(Location(temp.getPath()[k].getX(), temp.getPath()[k].getY()));
				if(map(location) == 1){
					if(fabs(temp.getPath()[k].getX()) - xObs < 2 * map.getGridResolution() && fabs(temp.getPath()[k].getY() - yObs < 2 * map.getGridResolution())) {
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

double Particle::probaByMove(const Deltas& deltas){
	double disntance = sqrt(pow(deltas.getX(), 2) + pow(deltas.getY(), 2));
	return helpFunc(disntance + fabs(deltas.getYaw()));

 }

vector<double> Particle::random_numbers(){
	vector<double> nums;
	nums.push_back(rand() %(2*RANGE+1)+(-RANGE));
	nums.push_back(rand() %(2*RANGE+1)+(-RANGE));
	nums.push_back(dtor(rand() %360+(-180)));
	return nums;
}

vector<Particle>::iterator Particle::find(vector<Particle>& list, const Particle& p) {
	for(vector<Particle>::iterator it = list.begin(); it != list.end(); ++it)
		if(p == *it)
			return it;
	return list.end();
}

vector <Particle> Particle::spawnParticles(int num, const Map& map) {
	vector<Particle> particles;

	srand(time(NULL));

	for(int i = 0; i < num; ++i) {
		vector<double> vars = random_numbers();
		Location location = map.getGridLocation(Location(getPosition().getX() + vars[0] * map.getGridResolution(), getPosition().getY() + vars[1] * map.getGridResolution()));

		if(map(location) != 1) {
			Particle newParticle = Particle(map, Position(getPosition().getX() + vars[0] * map.getGridResolution(), getPosition().getY() + vars[1] * map.getGridResolution(), getPosition().getYaw() + vars[2]), getBelief());

			if(particles.end() == find(particles, newParticle))
				particles.push_back(newParticle);
		}
	}

	return particles;
}

