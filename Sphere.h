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
	double r; // coefficient de réflexion
	
	// Constructeurs

	Sphere();
	Sphere(Vector center, double radius, Vector color, double r);

	// Destructeur
	
	~Sphere();
};

// Affichage console

ostream& operator<<(ostream& os, const Sphere& s);

// Surcharge de l'opérateur "=="

bool operator==(Sphere const& sphere1, Sphere const& sphere2);
