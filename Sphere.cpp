#include "Sphere.h"

using namespace std;


// Constructeurs

Sphere::Sphere() {

	center = Vector();
	radius = 0;
	color = Vector(); // black sphere by default
}

Sphere::Sphere(Vector center, double radius, Vector color) {

	this->center = center;
	this->radius = radius;
	this->color = color;
}


// Affichage console

ostream& operator<<(ostream& os, const Sphere& s) {
	
	os << "Sphere[center = " << s.center << "; radius = " << s.radius << "; couleur = " << s.color << "]";
	return os;
}


// Destructeur

Sphere::~Sphere() {

}
