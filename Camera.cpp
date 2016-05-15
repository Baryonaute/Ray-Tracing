#include "Camera.h"

using namespace std;


// Constructeurs

Camera::Camera() {
	
	eye = Vector();
	target = Vector();
	orientation = Vector();
	width = 0;
	height = 0;
}

Camera::Camera(Vector eye, Vector target, Vector orientation) {
	
	this->eye = eye;
	this->target = target;
	this->orientation = orientation;
	this->width = 640;
	this->height = 480;
}

Camera::Camera(Vector eye, Vector target, Vector orientation, int width, int height) {
	
	this->eye = eye;
	this->target = target;
	this->orientation = orientation;
	this->width = width;
	this->height = height;
}


// Affichage console

ostream& operator<<(ostream& os, const Camera& c) {
	
	os << "Camera[eye = " << c.eye << "; target = " << c.target << "; orientation = " << c.orientation << "; width = " << c.width << "; height = " << c.height << "]";
	return os;
}


// Destructeur

Camera::~Camera() {
	
}