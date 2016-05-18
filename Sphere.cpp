#include "Sphere.h"

using namespace std;


// Constructeurs

Sphere::Sphere() {

	center = Vector();
	radius = 0;
	color = Vector(); // black sphere by default
}

Sphere::Sphere(Vector center, double radius, Vector color, double r) {

	this->center = center;
	this->radius = radius;
	this->color = color;
	this->r = r;
}


// Affichage console

ostream& operator<<(ostream& os, const Sphere& s) {
	
	os << "Sphere[center = " << s.center << "; radius = " << s.radius << "; couleur = " << s.color << "; reflexion = " << s.r << "]";
	return os;
}


// Surcharge de l'opérateur "=="

bool operator==(Sphere const& sphere1, Sphere const& sphere2) {
	
	if (sphere1.center.x == sphere2.center.x && sphere1.center.y == sphere2.center.y
		&& sphere1.center.z == sphere2.center.z && sphere1.color.x == sphere2.color.x
		&& sphere1.color.y == sphere2.color.y && sphere1.color.z == sphere2.color.z
		&& sphere1.radius == sphere2.radius && sphere1.r == sphere2.r) return true;
	
	return false;
}

// Destructeur

Sphere::~Sphere() {

}
