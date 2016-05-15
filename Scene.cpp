#include "Scene.h"

using namespace std;


// Constructeur

Scene::Scene(vector<Sphere> spheres) {
	
	this->spheres = spheres;
}


// Affichage console

ostream& operator<<(ostream& os, const Scene& s) {
	
	long n = s.spheres.size();
	
	os << "Scene[";
	
	for (int i = 0; i < n - 1; ++i) {
		
		os << s.spheres[i] << ", ";
	}
	
	os << s.spheres[n - 1] << "]";
	return os;
}


// Destructeur

Scene::~Scene() {
	
}
