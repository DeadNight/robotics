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

void Partical::update(LaserProxy *lp,double dx, double dy, double dyaw, Map *map,double mapResolution){

	this->position.setLocation(Location(this->position.getX()+dx,this->position.getY()+dy));
	this->position.setYaw(this->position.getYaw()+dyaw);
	this->belief=NORMAL*this->belief*probaByMove(dx,dy,dyaw)*probaByLazer(lp,map,mapResolution);
	cout <<this->belief<< endl;
	cout <<"belief by move  " << probaByMove(dx,dy,dyaw)<< endl;

}

void Partical::printPartical(Map *_map,const char* mapFilePath, float mapResolution){
	Color red(255,0,0);
	double resolutionRatio = (double)_map->getGridResolution() / mapResolution;
	Image image = _map->toImage();
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
double Partical::probaByLazer(LaserProxy *lp, Map *_map,double mapResolution){
	double hits = 0;
	int cnt=0;
	double resolutionRatio = (double)_map->getGridResolution() / mapResolution;
	Image image = _map->toImage();
	Color yellow(0,255,0);
	Color two(0,0,255);
	Color three(200,200,50);
	Color four(255,0,0);
	for(uint i = 0; i < lp->GetCount(); ++i) {
		double d = resolutionRatio*(*lp)[i];
		//cout<<"d "<<d<<endl;
		double angle =  dtor(this->position.getYaw()) + lp->GetBearing(i);
		//cout<<"yaw: "<<this->position.getYaw()<<endl;
		double xObs = this->position.getX() + 2*d*cos(angle);
		double yObs = this->position.getY() - 2*d*sin(angle);
		cout<<"angle= "<<angle<<endl;
		Path temp=linearPath(Location(this->position.getX(),this->position.getY()),Location(xObs,yObs));

		if (temp.getPath().size()==0)
		{
			cout<<"lp "<<i<<endl;
		}
		for (unsigned z=0; z<temp.getPath().size();z++){
				image.setAround(Location(temp.getPath()[z].getX()*resolutionRatio,temp.getPath()[z].getY()*resolutionRatio),yellow);
		}
		image.setAround(Location(xObs*resolutionRatio,yObs*resolutionRatio),four);

		if (d < lp->GetMaxRange()){
			for (unsigned j =0 ; j <temp.getPath().size(); j++){
				//cout<< j<<"j:  "<<temp.getPath()[j].getX()<< ","<<temp.getPath()[j].getY()<< "= "<< (*_map)(temp.getPath()[j].getX(),temp.getPath()[j].getY())<<endl;
				if ((*_map)(temp.getPath()[j].getX(),temp.getPath()[j].getY())==1){
					if (fabs(temp.getPath()[j].getX())-xObs<1 && fabs(temp.getPath()[j].getY())-yObs<1)
					{
						hits++;
					}
					else if (fabs(temp.getPath()[j].getX())-xObs<2 && fabs(temp.getPath()[j].getY())-yObs<2)
					{
						hits+=0.9;
					}

					Location l(temp.getPath()[j].getX()*resolutionRatio,temp.getPath()[j].getY()*resolutionRatio);
					image.setAround(l,two);
					break;
				}
			}
		}
		else{
			bool check=false;
			for (unsigned k = 0; k <= temp.getPath().size(); k++){
				//cout<<k<< "k= "<< temp.getPath()[k].getX()<< ","<<temp.getPath()[k].getY()<< "= "<< (*_map)(temp.getPath()[k].getX(),temp.getPath()[k].getY())<<endl;
				if ((*_map)(temp.getPath()[k].getX(),temp.getPath()[k].getY())==1){
					if (fabs(temp.getPath()[k].getX())-xObs<2 && fabs(temp.getPath()[k].getY())-yObs<2)
					{
						hits+=0.9;
					}
					check=true;
					Location l(temp.getPath()[k].getX()*resolutionRatio,temp.getPath()[k].getY()*resolutionRatio);
					image.setAround(l,two);
					break;
				}
			}
			if (!check){
				cnt++;
				hits++;
				Location l(temp.getPath()[temp.getPath().size()-1].getX()*resolutionRatio,temp.getPath()[temp.getPath().size()-1].getY()*resolutionRatio);
				image.setAround(l,two);
			}
		}
	}
	image.setAround(Location(this->getPosition().getX()*resolutionRatio,this->getPosition().getY()*resolutionRatio),four);
	image.save("test.png");
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

