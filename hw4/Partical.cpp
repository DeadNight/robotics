/*
 * Partical.cpp
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#include "Partical.h"

Partical::Partical(Position p, double belief) {
	this->position=p;
	this->belief=belief;
}


double Partical::getBelief() const {
	return belief;
}

void Partical::setBelief(double belief) {
	this->belief = belief;
}

const Position& Partical::getPosition() const {
	return position;
}

void Partical::setPosition(const Position& position) {
	this->position= position;
}

void Partical::update(LaserProxy *lp,double dx, double dy, double dyaw, Map *map){

	this->position.setLocation(Location(this->position.getX()+dx,this->position.getY()+dy));
	this->position.setYaw(this->position.getYaw()+dyaw);
	this->belief=NORMAL*this->belief*probaByMove(dx,dy,dyaw)*probaByLazer(lp,map);
	cout <<this->belief<< endl;
	cout <<"belief by move  " << probaByMove(dx,dy,dyaw)<< endl;

}

void Partical::printPartical(Map *_map,const char* mapFilePath, float mapResolution){
	Color red(255,0,0);
	double resolutionRatio = (double)_map->getGridResolution() / mapResolution;
	Image image = _map->toImage(mapResolution);
	Location l;
	l.setX(position.getLocation().getX()*resolutionRatio);
	l.setY(position.getLocation().getY()*resolutionRatio);
	image.setAround(l,red);
	image.save(mapFilePath);
}

void Partical::printPartical(Map *_map,Image *image, float mapResolution){
	Color red(255,0,0);
	double resolutionRatio = (double)_map->getGridResolution() / mapResolution;
	Location l;
	l.setX(position.getLocation().getX()*resolutionRatio);
	l.setY(position.getLocation().getY()*resolutionRatio);
	image->setAround(l,red);
}

Path Partical::linearPath(Location from, Location to){
	unsigned top, bottom;
	unsigned fromY = from.getY();
	unsigned toY = to.getY();
	unsigned fromX = from.getX();
	unsigned toX = to.getX();
	std::vector<Location> temp;
		if(fromY < toY) {
			bottom = fromY; top = toY;
		} else {
			bottom = toY; top = fromY;
		}

		if(toX == fromX) {
			unsigned x = toX;
			for(unsigned y = bottom; y <= top; ++y){
				temp.push_back(Location(x,y));
			}
		} else {
			unsigned left, right;

			double m = ((double)toY - fromY)/((double)toX - fromX);
			double n = toY - m*toX;

			if(fromX < toX) {
				left = fromX; right = toX;
			} else {
				left = toX; right = fromX;
			}
			for(unsigned x = left; x <= right; ++x){
					for(unsigned y = bottom; y <= top; ++y){
						if(y == (unsigned)(m*x + n)){
							temp.push_back(Location(x,y));
						}
					}
			}
		}

		Path path(temp);
		return path;
}
//Function that returns values between 0 to 1
//The higher the value of X, the lower the value that the function returns
double Partical::helpFunc (double x){
	return 1-fabs(atan(x)/(PI/2));
}
double Partical::probaByLazer(LaserProxy *lp, Map *_map){
	double hits = 0;
	int cnt=0;
	for(uint i = 0; i < lp->GetCount(); ++i) {
		bool check1=false;
		double d = (*lp)[i];
		cout<<"d "<<d<<endl;
		double angle = this->position.getYaw() + lp->GetBearing(i);
		double xObs = this->position.getX() + d*cos(angle);
		double yObs = this->position.getY() + d*sin(angle);
		double virXObs = this->position.getX() + 2*d*cos(angle);
		double virYObs = this->position.getY() + 2*d*sin(angle);
		Path temp=linearPath(Location(this->position.getX(),this->position.getY()),Location(virXObs,virYObs));
		cout<<"xobs "<<xObs;
		cout<<", yobs "<<yObs<<"yaw "<<angle<<endl;
		cout<<"virxobs "<<virXObs;
		cout<<", viryobs "<<virYObs<<"yaw "<<angle<<endl;
		cout<<"xpos "<<this->position.getX();
		cout<<", ypos "<<this->position.getY()<<"yaw "<<angle<<endl;
		cout<<"path size: "<<temp.getPath().size()<<endl;
		for (unsigned t=0;t<temp.getPath().size();t++){
			cout<<"path x : " <<temp.getPath()[t].getX()<<" path y: "<<temp.getPath()[t].getY() <<endl;
		}
		if (d < lp->GetMaxRange()){
			for (unsigned j = temp.getPath().size()-1; j >0; j--){
				cout<< j<<"j:  "<<temp.getPath()[j].getX()<< ","<<temp.getPath()[j].getY()<< "= "<< (*_map)(temp.getPath()[j].getX(),temp.getPath()[j].getY())<<endl;
				if ((*_map)(temp.getPath()[j].getX(),temp.getPath()[j].getY())==1){
					double disntance = sqrt(pow(temp.getPath()[j].getX()-xObs,2)+pow(temp.getPath()[j].getY()-yObs,2));
					hits+=(helpFunc(disntance));
					cout<<"hits d>: = "<<hits<<endl;
					cout<<"distance>: = "<<disntance<<endl;
					cout<<"helpFunc>: = "<<helpFunc(disntance)<<endl;
					check1=true;
					break;
				}
			}
		}
		else{
			bool check=false;
			for (unsigned k = temp.getPath().size()-1; k >= temp.getPath().size()/2; k--){
				cout<<k<< "k= "<< temp.getPath()[k].getX()<< ","<<temp.getPath()[k].getY()<< "= "<< (*_map)(temp.getPath()[k].getX(),temp.getPath()[k].getY())<<endl;
				if ((*_map)(temp.getPath()[k].getX(),temp.getPath()[k].getY())==1){
					double disntance = sqrt(pow(temp.getPath()[k].getX()-xObs,2)+pow(temp.getPath()[k].getY()-yObs,2));
					hits+=(helpFunc(disntance));
					check=true;
					cout<<"hits d<: = "<<hits<<endl;
					break;
				}
			}
			if (!check){
				cnt++;
				hits++;
			}
		}
	}
	cout<<"cnt: = "<<cnt<<endl;
	cout<<"hits: = "<<hits<<endl;
	cout<<"counts: = "<<lp->GetCount()<<endl;
	cout<<"end: = "<<hits/lp->GetCount()<<endl;
	return hits/lp->GetCount();
}

double Partical::probaByMove(double dx, double dy, double dyaw){
	cout<< "dx: " << dx<< endl<< "dy: " << dy<< endl<< "dz: " << dyaw<< endl;
	double disntance = sqrt(pow(dx,2)+pow(dy,2));
	cout<< "distance: " << disntance<< endl;
	return helpFunc(disntance+fabs(dyaw));

 }

vector<double> Partical::random_numbers(){
	vector<double> nums;
	nums.push_back(rand() %(2*RANGE+1)+(-RANGE));
	nums.push_back(rand() %(2*RANGE+1)+(-RANGE));
	nums.push_back(dtor(rand() %360+(-180)));
	return nums;
}

vector<Partical*>::iterator Partical::find(vector<Partical*>& list, const Partical& p) {
	for(vector<Partical*>::iterator it = list.begin(); it != list.end(); ++it)
	{
		if(p == **it){
			return it;
		}
	}
	return list.end();
}

vector <Partical*> Partical::particleMulti(int num, Map *_map){
	srand (time(NULL));
	vector <Partical*> _particles;
	vector <double> vars = random_numbers();
	Partical* temp = new Partical(Position(this->getPosition().getX()+vars[0],this->getPosition().getY()+vars[1],this->getPosition().getYaw()+vars[2]),this->getBelief());
	for (int i=0; i<num;i++){
		while (_particles.end()!=find(_particles,*temp) || (*_map)(this->getPosition().getX()+vars[0],this->getPosition().getY()+vars[1])==1){
			vars = random_numbers();
			temp =new Partical(Position(this->getPosition().getX()+vars[0],this->getPosition().getY()+vars[1],this->getPosition().getYaw()+vars[2]),this->getBelief());
		}
		_particles.push_back(temp);
	}

	return _particles;
}

