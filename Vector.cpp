#include "Vector.h"

using namespace std;


// Constructeurs

Vector::Vector() {

    x = 0;
    y = 0;
    z = 0;
}

Vector::Vector(double x, double y, double z) {

    this->x = x;
    this->y = y;
    this->z = z;
}


// Opérations élémentaires

Vector Vector::add(Vector v) {
	
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::substract(Vector v) {
	
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::multiply(double l) {
	
	return Vector(x * l, y * l, z * l);
}

double Vector::scalar(Vector v) {
	
	return (x * v.x + y * v.y + z * v.z);
}

Vector Vector::vectorial(Vector v) {
	
	return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.z);
}

double Vector::norm() {
	
	double n = sqrt(x * x + y * y + z * z);
	return n;
}

void Vector::normalize() {
	
	double n = this->norm();
	
	if (n == 0) {
		
		std::cout << "This vector is null. It can't be normalized.\n";
		
	} else {
		
		x = x / n;
		y = y / n;
		z = z / n;
	}
}


// Opérateurs surchargés

Vector operator+(Vector const& v, Vector const& w) { // addition
	
	Vector copy = v;
	Vector result = copy.add(w);
	return result;
}

Vector operator-(Vector const& v, Vector const& w) { // substraction
	
	Vector copy = v;
	Vector result = copy.substract(w);
	return result;
}

double operator*(Vector const& v, Vector const& w) { // produit scalaire
	
	Vector copy = v;
	double result = copy.scalar(w);
	return result;
}

Vector operator*(Vector const& v, double const& l) { // multiplication par un scalaire
	
	Vector copy = v;
	Vector result = copy.multiply(l);
	return result;
}

Vector operator*(double const& l, Vector const& v) { // multiplication par un scalaire (bis)
	
	Vector copy = v;
	Vector result = copy.multiply(l);
	return result;
}

Vector operator^(Vector const& v, Vector const& w) { // produit vectoriel
	
	Vector copy = v;
	Vector result = copy.vectorial(w);
	return result;
}


// Affichage console

ostream& operator<<(ostream& os, const Vector& v) {
	
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}


// Destructeur

Vector::~Vector() {

}
