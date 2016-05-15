#pragma once
#include <math.h>
#include <iostream>
#include "Vector.h"

using namespace std;

class Color {
	
	public :
	
	// Coordonnées
	
	Vector rgb;
	double alpha;
	
	// Constructeurs
	
	Color();
	Color(Vector rgb, double alpha);
	
	// Destructeur
	
	~Color();
};

// Affichage console

ostream& operator<<(ostream& os, const Color& c);