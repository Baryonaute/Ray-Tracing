#pragma once
#include <math.h>
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "Ray.h"

using namespace std;

class RayTracer {
	
	public :
	
	// Setup
	
	Camera camera;
	Scene scene;
	
	// Coefficients du modèle de Phong
	
	double ka; // Phong model : ambient light coeff
	double kd; // Phong model : diffuse light coeff
	double ks; // Phong model : specular light coeff
	double alpha; // Phong model : alpha coeff
	
	// Constructeur
	
	RayTracer(Camera camera, Scene scene, double ka, double kd, double ks, double alpha);
	
	// Calcul de la couleur d'un pixel en RGB
	
	Vector pixelCompute(Ray ray, Sphere sphere, Vector point);
	
	// Destructeur
	
	~RayTracer();
};

// Affichage console

ostream& operator<<(ostream& os, const RayTracer& r);
