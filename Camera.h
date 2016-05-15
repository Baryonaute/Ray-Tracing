#pragma once
#include <math.h>
#include <iostream>
#include "Vector.h"

using namespace std;

class Camera {
	
	public :
	
	// Coordonnées
	
	Vector eye;
	Vector target;
	Vector orientation;
	int width;
	int height;
	
	// Constructeurs
	
	Camera();
	Camera(Vector eye, Vector target, Vector orientation);
	Camera(Vector eye, Vector target, Vector orientation, int width, int height);
	
	// Destructeur
	
	~Camera();
};

// Affichage console

ostream& operator<<(ostream& os, const Camera& c);