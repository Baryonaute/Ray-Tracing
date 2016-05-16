#pragma once
#include <math.h>
#include <iostream>
#include "Vector.h"

using namespace std;

class Light {
	
	public :
	
	// Coordonnées
	
	Vector source;
	Vector color; // TODO : remplacer par un objet de la classe Color
	
	// Constructeurs
	
	Light();
	Light(Vector source);
	Light(Vector source, Vector color);
	
	// Destructeur
	
	~Light();
};

// Affichage console

ostream& operator<<(ostream& os, const Light& l);
