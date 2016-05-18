#include "RayTracer.h"

using namespace std;


// Constructeur

RayTracer::RayTracer(Camera camera, Scene scene, int max_depth, double ka, double kd, double ks, double alpha) {
	
	this->camera = camera;
	this->scene = scene;
	this->max_depth = max_depth; // Niveau de récursivité pour le calcul de la réflexion
	this->ka = ka; // Phong model : ambient light coeff
	this->kd = kd; // Phong model : diffuse light coeff
	this->ks = ks; // Phong model : specular light coeff
	this->alpha = alpha; // Phong model : alpha coeff
}


// Calcul de la couleur d'un pixel en RGB

Vector RayTracer::pixelCompute(Ray ray, Sphere sphere, Vector point) {
	
	Vector n = point - sphere.center;
	n.normalize(); // "n" est la normale à "sphere" en "point"
	
	Vector v = camera.eye - point;
	v.normalize(); // "v" est le vecteur unitaire qui va de "point" vers l'oeil
	
	Vector l; // "l" est le vecteur unitaire qui va de "point" vers la source lumineuse
	Vector r; // "r" est le vecteur unitaire symétrique de "l" par rapport à "n" (et coplanaire avec "l" et "n")
	
	Vector intensity = Vector();
	
	double numberOfLightSources = scene.lights.size();
	
	for (vector<Light>::iterator light = scene.lights.begin(); light != scene.lights.end(); ++light) {
		
		l = (*light).source - point;
		l.normalize();
		
		r = 2 * (l * n) * n - l;
		r.normalize();
		
		bool shadow = false;
		
		for (vector<Sphere>::iterator sph = scene.spheres.begin(); sph != scene.spheres.end(); ++sph) {
			
			Ray incident = Ray(point, l);
			
			if (incident.intersect(*sph).first && !(*sph == sphere)) shadow = true;
		}
		
		if (shadow) {
			
			intensity = (intensity + ka * (1 / numberOfLightSources) * (*light).color); // ambiant light
			
		} else {
			
			intensity = intensity + ka * (1 / numberOfLightSources) * (*light).color; // ambiant light
			if (l * n > 0) intensity = intensity + kd * (l * n) * (*light).color; // diffuse light
			if (r * v > 0) intensity = intensity + ks * pow((r * v), alpha) * (*light).color; // specular light
		}
	}
	
	double intensity_x = intensity.x * sphere.color.x / 255;
	double intensity_y = intensity.y * sphere.color.y / 255;
	double intensity_z = intensity.z * sphere.color.z / 255;
	
	if (intensity_x > 255) intensity_x = 255; // max value for the RGB color scale
	if (intensity_y > 255) intensity_y = 255;
	if (intensity_z > 255) intensity_z = 255;
	
	Vector result = Vector((unsigned char)(intensity_x), (unsigned char)(intensity_y), (unsigned char)(intensity_z)); // RGB colors of the pixel
	
	return result;
}

Vector RayTracer::recursivePixelCompute(Ray ray, int depth) {
	
	Vector result = Vector(0, 0, 0);
	
	if (depth > max_depth) return Vector(0, 0, 0);
	
	pair<bool, pair<Sphere, Vector>> intersection = ray.intersects(scene.spheres);
	Sphere sphere = intersection.second.first;
	Vector point = intersection.second.second;
	
	if (intersection.first) {
		
		result = result + pixelCompute(ray, sphere, point);
		
		if (sphere.r > 0) {
			
			Vector n = point - sphere.center;
			n.normalize();
			
			Ray reflected = Ray(point - 0.0001 * ray.direction, -2 * (ray.direction * n) * n + ray.direction);
			
			Vector tmp = recursivePixelCompute(reflected, depth + 1);
			
			if (tmp.x != 0 || tmp.y != 0 || tmp.z != 0) {
				
				result = (1 - sphere.r) * result + sphere.r * recursivePixelCompute(reflected, depth + 1); // on applique le coefficient de réflexion
				
			}
		}
	}
	
	return result;
}

// Affichage console

ostream& operator<<(ostream& os, const RayTracer& r) {
	
	os << "PhongCoefficients[ka = " << r.ka << "; kd = " << r.kd << "; ks = " << r.ks << "; alpha = " << r.alpha << "]";
	return os;
}


// Destructeur

RayTracer::~RayTracer() {
	
}
