#pragma once
#include <math.h>
#include <iostream>

using namespace std;

class Vector {
	
	public :
	
	// Coordonnées
	
	double x;
	double y;
	double z;
	
	// Constructeurs

	Vector();
	Vector(double x, double y, double z);

	// Opérations élémentaires

	Vector add(Vector v);
	Vector substract(Vector v);
	Vector multiply(double d);
	Vector vectorial(Vector v);
	double scalar(Vector v);
	double norm();
	void normalize();

	// Destructeur
	
	~Vector();
};

// Affichage console

ostream& operator<<(ostream& os, const Vector& v);

// Opérateurs surchargés

bool operator==(Vector const& vector1, Vector const& vector2); // égalité
Vector operator+(Vector const& v, Vector const& w); // addition
Vector operator-(Vector const& v, Vector const& w); // soustraction
double operator*(Vector const& v, Vector const& w); // produit scalaire
Vector operator*(Vector const& v, double const& l); // multiplication par un scalaire
Vector operator*(double const& l, Vector const& v); // multiplication par un scalaire
