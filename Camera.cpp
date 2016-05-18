#include "Camera.h"

using namespace std;


// Constructeurs

Camera::Camera() {
	
	this->eye = Vector(640*2, 480*2, 1000*2);
	this->target = -400 * Vector(0, 0, 1);
	this->orientation = Vector(0, 0, 1);
	this->width = 1280*2;
	this->height = 960*2;
}

Camera::Camera(Vector eye, Vector target, Vector orientation) {
	
	this->eye = eye;
	this->target = target;
	orientation.normalize();
	this->orientation = orientation;
	this->width = 640;
	this->height = 480;
}

Camera::Camera(Vector eye, Vector target, Vector orientation, int width, int height) {
	
	this->eye = eye;
	this->target = target;
	orientation.normalize();
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
