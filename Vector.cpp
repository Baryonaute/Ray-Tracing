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
		
		std::cout << "Program intended to normalize a null vector" << endl;
		
	} else {
		
		x = x / n;
		y = y / n;
		z = z / n;
	}
}


// Opérateurs surchargés

bool operator==(Vector const& vector1, Vector const& vector2) { // égalité
	
	if (vector1.x == vector2.x && vector1.y == vector2.y && vector1.z == vector2.z) return true;
	
	return false;
}

Vector operator+(Vector const& v, Vector const& w) { // addition
	
	Vector copy = v;
	Vector result = copy.add(w);
	return result;
}

Vector operator-(Vector const& v, Vector const& w) { // soustraction
	
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


// Affichage console

ostream& operator<<(ostream& os, const Vector& v) {
	
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}


// Destructeur

Vector::~Vector() {

}
