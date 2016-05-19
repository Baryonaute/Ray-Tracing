#include "Camera.h"

using namespace std;


// Constructeurs

Camera::Camera() {
	
	this->eye = Vector(1280, 100000, 0);
	this->target = Vector(1280, 960, -400);
	this->up = Vector(0, 0, -1);
	this->width = 2560;
	this->height = 1920;
}

Camera::Camera(Vector eye, Vector target, Vector up) {
	
	this->eye = eye;
	this->target = target;
	
	Vector eye_target = target - eye;
	eye_target.normalize();
	
	up = up - (eye_target * (eye_target * up));
	up.normalize();
	
	this->up = up;
	this->width = 2560;
	this->height = 1920;
}

Camera::Camera(Vector eye, Vector target, Vector up, int width, int height) {
	
	this->eye = eye;
	this->target = target;
	
	Vector eye_target = target - eye;
	eye_target.normalize();
	
	up = up - (eye_target * (eye_target * up));
	up.normalize();
	
	this->up = up;
	this->width = width;
	this->height = height;
}


// Affichage console

ostream& operator<<(ostream& os, const Camera& c) {
	
	os << "Camera[eye = " << c.eye << "; target = " << c.target << "; up = " << c.up << "; width = " << c.width << "; height = " << c.height << "]";
	return os;
}


// Destructeur

Camera::~Camera() {
	
}
