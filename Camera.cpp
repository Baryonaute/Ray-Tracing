#include "Camera.h"

using namespace std;


// Constructeurs

Camera::Camera() {
	
	Vector i = Vector(1, 0, 0);
	Vector j = Vector(0, 1, 0);
	Vector k = i.vectorial(j);
	
	this->eye = Vector(640, 480, 1000);
	this->target = -400 * k;
	this->orientation = k;
	this->width = 1280;
	this->height = 960;
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
