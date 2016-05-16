#pragma once
#include <math.h>
#include <iostream>
#include "Vector.h"

using namespace std;

class Sphere {
	
	public :
	
	// Coordonnées
	
	Vector center;
	double radius;
	Vector color;
	
	// Constructeurs

	Sphere();
	Sphere(Vector center, double radius, Vector color);

	// Destructeur
	
	~Sphere();
};

// Affichage console

ostream& operator<<(ostream& os, const Sphere& s);
