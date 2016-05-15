#include "Color.h"

using namespace std;


// Constructeurs

Color::Color() {
	
	rgb = Vector(); // (0, 0, 0) = black
	alpha = 0; // transparence
}

Color::Color(Vector rgb, double alpha) {
	
	rgb.x = (int)rgb.x;
	rgb.y = (int)rgb.y;
	rgb.z = (int)rgb.z;
	
	if (rgb.x > 255) {
		
		rgb.x = 255;
	}
	
	if (rgb.y > 255) {
		
		rgb.y = 255;
	}
	
	if (rgb.z > 255) {
		
		rgb.z = 255;
	}
	
	if (alpha < 0) alpha = 0;
	if (alpha > 1) alpha = 1;
	
	this->rgb = Vector(rgb.x, rgb.y, rgb.z);
	this->alpha = alpha;
}


// Affichage console

ostream& operator<<(ostream& os, const Color& c) {
	
	os << "Color[rgb = " << c.rgb << "; alpha = " << c.alpha << "]";
	return os;
}


// Destructeur

Color::~Color() {
	
}