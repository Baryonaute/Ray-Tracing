#pragma once
#include <math.h>
#include <iostream>
#include <vector>
#include "Sphere.h"

using namespace std;

class Scene {
	
	public :
	
	// Container
	
	vector<Sphere> spheres;
	
	// Constructeur
	
	Scene(vector<Sphere> spheres);
	
	// Destructeur
	
	~Scene();
};

// Affichage console

ostream& operator<<(ostream& os, const Scene& s);