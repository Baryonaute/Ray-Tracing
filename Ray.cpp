#include "Ray.h"

using namespace std;


// Constructeurs

Ray::Ray() {
	
	point = Vector();
	direction = Vector();
}

Ray::Ray(Vector point, Vector direction) {
	
	this->point = point;
	this->direction = direction;
}


// Affichage console

ostream& operator<<(ostream& os, const Ray& r) {
	
	os << "Ray[origin = " << r.point << "; direction = " << r.direction << "]";
	return os;
}


// Destructeur

Ray::~Ray() {
	
}