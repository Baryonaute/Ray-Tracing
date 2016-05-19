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
	n.normalize();
	
	Vector v = camera.eye - point;
	v.normalize();
	
	Vector l;
	Vector r;
	
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
			
			intensity = (intensity + ka * (1 / numberOfLightSources) * (*light).color); // lumière ambiante
			
		} else {
			
			intensity = intensity + ka * (1 / numberOfLightSources) * (*light).color; // lumière ambiante
			if (l * n > 0) intensity = intensity + kd * (l * n) * (*light).color; // lumière diffuse
			if (r * v > 0) intensity = intensity + ks * pow((r * v), alpha) * (*light).color; // lumière "specular"
		}
	}
	
	double intensity_x = intensity.x * sphere.color.x / 255;
	double intensity_y = intensity.y * sphere.color.y / 255;
	double intensity_z = intensity.z * sphere.color.z / 255;
	
	if (intensity_x > 255) intensity_x = 255;
	if (intensity_y > 255) intensity_y = 255;
	if (intensity_z > 255) intensity_z = 255;
	
	Vector result = Vector((unsigned char)(intensity_x), (unsigned char)(intensity_y), (unsigned char)(intensity_z));
	
	return result;
}


// Calcul récursif de la couleur d'un pixel en RGB pour ajouter les effets de réflexion

Vector RayTracer::recursivePixelCompute(Ray ray, int depth) {
	
	Vector result = Vector();
	
	if (depth > max_depth) return Vector();
	
	pair<bool, pair<Sphere, Vector> > intersection = ray.intersects(scene.spheres);
	Sphere sphere = intersection.second.first;
	Vector point = intersection.second.second;
	
	if (intersection.first) {
		
		result = result + pixelCompute(ray, sphere, point);
		
		if (sphere.r > 0) {
			
			Vector n = point - sphere.center;
			n.normalize();
			
			Ray reflected = Ray(point - 0.0001 * ray.direction, -2 * (ray.direction * n) * n + ray.direction);
			
			/* Pour des raisons d'arrondis numériques, on est obligé de prendre comme point de départ du rayon réfléchi un point légèrement (-0.0001 * ray.direction est complètement arbitraire !) en retrait du point d'intersection calculé. On obtenait sinon des effets moirés particulièrement frustrants : le programme considérait que certains points étaient DANS la sphère et donc que le rayon coupait immédiatement la sphère courante, sans aller plus loin.
			 */
			
			Vector tmp = recursivePixelCompute(reflected, depth + 1);
			
			if (!(tmp == Vector(0, 0, 0))) {
				
				result = (1 - sphere.r) * result + sphere.r * recursivePixelCompute(reflected, depth + 1); // sphere.r = coefficient de réflexion
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
