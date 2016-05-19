#pragma once
#include <math.h>
#include <iostream>
#include "Vector.h"

using namespace std;

class Light {
	
	public :
	
	// Coordonnées
	
	Vector source;
	Vector color; // Vector(R, G, B)
	
	// Constructeurs
	
	Light();
	Light(Vector source);
	Light(Vector source, Vector color);
	
	// Destructeur
	
	~Light();
};

// Affichage console

ostream& operator<<(ostream& os, const Light& l);
