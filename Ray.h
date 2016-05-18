#pragma once
#include <math.h>
#include <iostream>
#include <vector>
#include "Vector.h"
#include "Sphere.h"

using namespace std;

class Ray {
	
	public :
	
	// Coordonnées
	
	Vector point;
	Vector direction;
	
	// Constructeurs
	
	Ray();
	Ray(Vector point, Vector direction);
	
	// Intersection avec une sphère
	
	pair<bool, Vector> intersect(Sphere s);
	pair<bool, pair<Sphere, Vector>> intersects(vector<Sphere> spheres);
	
	// Destructeur
	
	~Ray();
};

// Affichage console

ostream& operator<<(ostream& os, const Ray& r);
