#include "Scene.h"

using namespace std;


// Constructeurs

Scene::Scene() {
	
	vector<Sphere> spheres;
	spheres.push_back(Sphere());
	vector<Light> lights;
	lights.push_back(Light());
	
	this->spheres = spheres;
	this->lights = lights;
}

Scene::Scene(vector<Sphere> spheres, vector<Light> lights) {
	
	this->spheres = spheres;
	this->lights = lights;
}


// Affichage console

ostream& operator<<(ostream& os, const Scene& s) {
	
	long n = s.spheres.size();
	long m = s.lights.size();
	
	os << "Scene[";
	for (int i = 0; i < n; ++i) os << s.spheres[i] << ", ";
	for (int j = 0; j < m - 1; ++j) os << s.lights[j] << ", ";
	os << s.lights[n - 1] << "]";
	
	return os;
}


// Destructeur

Scene::~Scene() {
	
	
}
