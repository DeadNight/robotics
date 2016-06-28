/*
 * Partical.cpp
 *
 *  Created on: May 31, 2016
 *      Author: colman
 */

#include "Partical.h"

Partical::Partical(Map *_map,Position p, double belief) {
	p.setLocation(_map->getWorldLocation(_map->getGridPosition(p).getLocation()));
	setPosition(p);
	setBelief(belief);
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

void Partical::update(LaserProxy *lp,double dx, double dy, double dyaw, Map *_map){

	this->position.setLocation(Location(this->position.getX()+dx,this->position.getY()+dy));
	this->position.setYaw(this->position.getYaw()+dyaw);
	this->belief=NORMAL*this->belief*probaByMove(dx,dy,dyaw)*probaByLazer(lp,_map);
	cout <<this->belief<< endl;
	cout <<"belief by move  " << probaByMove(dx,dy,dyaw)<< endl;

}

void Partical::printPartical(Map *_map,const char* mapFilePath){
	Image image = _map->toImage();
	Location l;
	l.setX(position.getLocation().getX()/ _map->getMapResolution());
	l.setY(position.getLocation().getY()/ _map->getMapResolution());
	image.setAround(l,Color::Red);
	image.save(mapFilePath);
}

void Partical::printPartical(Map *_map,Image *image){
	Location l;
	l.setX(position.getLocation().getX()/ _map->getMapResolution());
	l.setY(position.getLocation().getY() / _map->getMapResolution());
	image->setAround(l,Color::Red);
}
Path Partical::linearPath(Location from, Location to){

	double x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
	std::vector<Location> temp;
	double x1 = from.getX();
	double x2 = to.getX();
	double y1 = from.getY();
	double y2 = to.getY();

	 dx=x2-x1;
	 dy=y2-y1;
	 dx1=fabs(dx);
	 dy1=fabs(dy);
	 px=2*dy1-dx1;
	 py=2*dx1-dy1;
	 if(dy1<=dx1)
	 {
	  if(dx>=0)
	  {
	   x=x1;
	   y=y1;
	   xe=x2;
	  }
	  else
	  {
	   x=x2;
	   y=y2;
	   xe=x1;
	  }
	  temp.push_back(Location(x,y));
	  for(i=0;x<xe;i++)
	  {
	   x=x+1;
	   if(px<0)
	   {
	    px=px+2*dy1;
	   }
	   else
	   {
	    if((dx<0 && dy<0) || (dx>0 && dy>0))
	    {
	     y=y+1;
	    }
	    else
	    {
	     y=y-1;
	    }
	    px=px+2*(dy1-dx1);
	   }
	   temp.push_back(Location(x,y));
	  }
	 }
	 else
	 {
	  if(dy>=0)
	  {
	   x=x1;
	   y=y1;
	   ye=y2;
	  }
	  else
	  {
	   x=x2;
	   y=y2;
	   ye=y1;
	  }
	  temp.push_back(Location(x,y));
	  for(i=0;y<ye;i++)
	  {
	   y=y+1;
	   if(py<=0)
	   {
	    py=py+2*dx1;
	   }
	   else
	   {
	    if((dx<0 && dy<0) || (dx>0 && dy>0))
	    {
	     x=x+1;
	    }
	    else
	    {
	     x=x-1;
	    }
	    py=py+2*(dx1-dy1);
	   }
	   temp.push_back(Location(x,y));
	  }
	 }
	 if (temp[0].getX()!=from.getX() || temp[0].getY()!=from.getY())
	 {
		 std::reverse(temp.begin(),temp.end());
	 }
	 return(Path(temp));
}

//Function that returns values between 0 to 1
//The higher the value of X, the lower the value that the function returns
double Partical::helpFunc (double x){
	return 1-fabs(atan(x)/(PI/2));
}
double Partical::probaByLazer(LaserProxy *lp, Map *_map){
	double hits = 0;
	int cnt=0;
	//Image image = _map->toImage();

	for(uint i = 0; i < lp->GetCount(); ++i) {
		double d = 100*(*lp)[i];
		//cout<<"d "<<d<<endl;
		double angle =  dtor(this->position.getYaw()) + lp->GetBearing(i);
		//cout<<"yaw: "<<this->position.getYaw()<<endl;
		double xObs = this->position.getX() + d*cos(angle);
		double yObs = this->position.getY() - d*sin(angle);

		Path temp=linearPath(Location(this->position.getX(),this->position.getY()),Location(xObs,yObs));


		/*for (unsigned z=0; z<temp.getPath().size();z++){
				image.setAround(Location(temp.getPath()[z].getX()/ _map->getMapResolution(),temp.getPath()[z].getY()/ _map->getMapResolution()),Color::Green);
		}
		image.setAround(Location(xObs/ _map->getMapResolution(),yObs/ _map->getMapResolution()),Color::Magenta);*/

		if (d < 100*lp->GetMaxRange()){
			for (unsigned j =0 ; j <temp.getPath().size(); j++){
				//cout<< j<<"j:  "<<temp.getPath()[j].getX()<< ","<<temp.getPath()[j].getY()<< "= "<< (*_map)(temp.getPath()[j].getX(),temp.getPath()[j].getY())<<endl;
				Location location= _map->getGridLocation(Location(temp.getPath()[j].getX(),temp.getPath()[j].getY()));
				if ((*_map)(location)==1){
					/*cout<<"x-obs= "<<fabs(temp.getPath()[j].getX()-xObs)<<endl;
					cout<<"yobs= "<<fabs(temp.getPath()[j].getY()-xObs)<<endl;*/
					if (fabs(temp.getPath()[j].getX()-xObs)<1*_map->getGridResolution() && fabs(temp.getPath()[j].getY()-yObs)<1*_map->getGridResolution())
					{
						hits++;
					}
					else if (fabs(temp.getPath()[j].getX()-xObs)<2*_map->getGridResolution() && fabs(temp.getPath()[j].getY()-yObs<2*_map->getGridResolution()))
					{
						hits+=0.9;
					}

/*					Location l(temp.getPath()[j].getX()/_map->getMapResolution(),temp.getPath()[j].getY()/ _map->getMapResolution());
					image.setAround(l,Color::Blue);*/
					break;
				}
			}
		}
		else{
			bool check=false;
			for (unsigned k = 0; k <= temp.getPath().size(); k++){
				//cout<<k<< "k= "<< temp.getPath()[k].getX()<< ","<<temp.getPath()[k].getY()<< "= "<< (*_map)(temp.getPath()[k].getX(),temp.getPath()[k].getY())<<endl;
				Location location= _map->getGridLocation(Location(temp.getPath()[k].getX(),temp.getPath()[k].getY()));
				/*cout<<"x-obs= "<<fabs(temp.getPath()[k].getX()-xObs)<<endl;
				cout<<"y-obs= "<<fabs(temp.getPath()[k].getY()-xObs)<<endl;*/
				if ((*_map)(location)==1){
					if (fabs(temp.getPath()[k].getX())-xObs<2*_map->getGridResolution() && fabs(temp.getPath()[k].getY()-yObs<2*_map->getGridResolution()))
					{
						hits+=0.9;
					}
					check=true;
					/*Location l(temp.getPath()[k].getX()/_map->getMapResolution(),temp.getPath()[k].getY()/ _map->getMapResolution());
					image.setAround(l,Color::Blue);*/
					break;
				}
			}
			if (!check){
				cnt++;
				hits++;
				/*Location l(temp.getPath()[temp.getPath().size()-1].getX()/_map->getMapResolution(),temp.getPath()[temp.getPath().size()-1].getY()/ _map->getMapResolution());
				image.setAround(l,Color::Blue);*/
			}
		}
	}
	/*image.setAround(Location(this->getPosition().getX()/ _map->getMapResolution(),this->getPosition().getY()/ _map->getMapResolution()),Color::Red);
	image.save("test.png");*/
	cout<<"cnt: = "<<cnt<<endl;
	cout<<"hits: = "<<hits<<endl;
	cout<<"counts: = "<<lp->GetCount()<<endl;
	cout<<"end: = "<<hits/lp->GetCount()<<endl;
	return hits/lp->GetCount();
}

double Partical::probaByMove(double dx, double dy, double dyaw){
	double disntance = sqrt(pow(dx,2)+pow(dy,2));
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
	Partical* temp =new Partical(_map,Position(this->getPosition().getX()+vars[0]*_map->getGridResolution(),this->getPosition().getY()+vars[1]*_map->getGridResolution(),this->getPosition().getYaw()+vars[2]),this->getBelief());

	for (int i=0; i<num;i++){
		Location location= _map->getGridLocation(Location(this->getPosition().getX()+vars[0]*_map->getGridResolution(),this->getPosition().getY()+vars[1]*_map->getGridResolution()));
		while (_particles.end()!=find(_particles,*temp) || (*_map)(location)==1){
			vars = random_numbers();
			temp =new Partical(_map,Position(this->getPosition().getX()+vars[0]*_map->getGridResolution(),this->getPosition().getY()+vars[1]*_map->getGridResolution(),this->getPosition().getYaw()+vars[2]),this->getBelief());

		}
		_particles.push_back(temp);
	}

	return _particles;
}

