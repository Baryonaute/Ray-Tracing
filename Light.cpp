#include "Light.h"

using namespace std;


// Constructeurs

Light::Light() {
	
	source = Vector();
	color = Vector(255, 255, 255); // white light by default
}

Light::Light(Vector source) {
	
	this->source = source;
	color = Vector(255, 255, 255); // white light by default
}

Light::Light(Vector source, Vector color) {
	
	this->source = source;
	this->color = color;
}


// Affichage console

ostream& operator<<(ostream& os, const Light& l) {
	
	os << "Light[source = " << l.source << "; color = " << l.color << "]";
	return os;
}


// Destructeur

Light::~Light() {
	
}
