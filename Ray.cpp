#include "Ray.h"

using namespace std;


// Constructeurs

Ray::Ray() {
	
	point = Vector();
	direction = Vector();
}

Ray::Ray(Vector point, Vector direction) {
	
	this->point = point;
	direction.normalize();
	//cout << "Ray_Direction error : " << direction << endl;
	this->direction = direction;
}


// Intersection avec une sphère

pair<bool, Vector> Ray::intersect(Sphere s) {
	
	Vector p = this->point;
	Vector d = this->direction; // normé par construction
	Vector c = s.center;
	double r = s.radius;
	
	Vector v = p - c;
	Vector result = Vector();
	
	double delta = 4 * ((v * d) * (v * d) - (v * v - r * r));
	
	if (delta < 0) {
		
		return pair<bool, Vector>(false, result);
		
	} else if (delta == 0) {
		
		double doubleRoot = - (v * d);
		
		if (doubleRoot < 0) {
			
			return pair<bool, Vector>(false, result);
			
		} else {
			
			result = p + doubleRoot * d;
			
		}
		
	} else {
		
		double root1 = - (v * d) + sqrt(delta) / 2;
		double root2 = - (v * d) - sqrt(delta) / 2;
		
		if (root1 < 0 && root2 < 0) {
			
			return pair<bool, Vector>(false, result);
			
		} else if (root1 > 0 && root2 > 0) {
			
			if (root1 < root2) {
				
				result = p + root1 * d;
				
			} else {
				
				result = p + root2 * d;
				
			}
			
		} else {
			
			if (root1 >= 0) {
				
				result = p + root1 * d;
				
			} else {
				
				result = p + root2 * d;
				
			}
			
		}
	}
	
	return pair<bool, Vector>(true, result);
}

// Affichage console

ostream& operator<<(ostream& os, const Ray& r) {
	
	os << "Ray[origin = " << r.point << "; direction = " << r.direction << "]";
	return os;
}


// Destructeur

Ray::~Ray() {
	
}
