#pragma once
#include <math.h>
#include <iostream>
#include <vector>
#include "Sphere.h"
#include "Light.h"

using namespace std;

class Scene {
	
	public :
	
	// Objets
	
	vector<Sphere> spheres;
	vector<Light> lights;
	
	// Constructeur
	
	Scene();
	Scene(vector<Sphere> spheres, vector<Light> lights);
	
	// Destructeur
	
	~Scene();
};

// Affichage console

ostream& operator<<(ostream& os, const Scene& s);
