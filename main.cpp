#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Camera.h"
#include "Light.h"
#include "Ray.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector.h"

using namespace std;

struct RGBType {
	
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

void savebmp (const char *filename, int width, int height, int dpi, RGBType *data) {
	
	FILE *f;
	int k = width * height;
	int s = 4 * k;
	int filesize = 54 + s;
	
	double factor = 39.375;
	int m = static_cast<int>(factor);
	
	int ppm = dpi * m;
	
	unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
	unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
	
	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);
	
	bmpinfoheader[4] = (unsigned char)(width);
	bmpinfoheader[5] = (unsigned char)(width >> 8);
	bmpinfoheader[6] = (unsigned char)(width >> 16);
	bmpinfoheader[7] = (unsigned char)(width >> 24);
	
	bmpinfoheader[8] = (unsigned char)(height);
	bmpinfoheader[9] = (unsigned char)(height >> 8);
	bmpinfoheader[10] = (unsigned char)(height >> 16);
	bmpinfoheader[11] = (unsigned char)(height >> 24);
	
	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);
	
	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);
	
	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);
	
	f = fopen(filename, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	
	for (int i = 0; i < k; i++) {
		
		RGBType rgb = data[i];
		
		unsigned char red = rgb.r;
		unsigned char green = rgb.g;
		unsigned char blue = rgb.b;
		
		unsigned char color[3] = {blue, green, red};
		
		fwrite(color, 1, 3, f);
	}
	
	fclose(f);
}

int main(int argc, const char * argv[]) {
	
	///////////////////////////////////////
	//// MISE EN PLACE DE L'EXPERIENCE ////
	///////////////////////////////////////
	
	cout << endl;
	cout << "///////////////////////////////////////" << endl;
	cout << "//// MISE EN PLACE DE L'EXPERIENCE ////" << endl;
	cout << "///////////////////////////////////////" << endl;
	cout << endl;
	
	ifstream inputFile;
	string fileLocation = "./scene1.txt"; // choisir la scène désirée
	inputFile.open(fileLocation);
	
	while(inputFile.fail()) {
		
		cout << "The file at location " << fileLocation << " failed to open" << endl;
		return 1;
	}
	
	
	// Lights
	
	int nbLights;
	inputFile >> nbLights;
	
	vector<Light> lights;
	
	for (int i = 0; i < nbLights; ++i) {
		
		double x, y, z, red, green, blue;
		inputFile >> x >> y >> z >> red >> green >> blue;
		lights.push_back(Light(Vector(x, y, z), Vector(red, green, blue)));
	}
	
	
	// Spheres
	
	int nbSpheres;
	inputFile >> nbSpheres;
	
	vector<Sphere> spheres;
	
	for (int i = 0; i < nbSpheres; ++i) {
		
		double x, y, z, radius, red, green, blue, reflexion;
		inputFile >> x >> y >> z >> radius >> red >> green >> blue >> reflexion;
		spheres.push_back(Sphere(Vector(x, y, z), radius, Vector(red, green, blue), reflexion));
	}
	
	
	// Scène
	
	Scene scene = Scene(spheres, lights);
	
	
	// Caméra
	
	double x_eye, y_eye, z_eye, x_target, y_target, z_target, x_up, y_up, z_up;
	int w, h;
	inputFile >> x_eye >> y_eye >> z_eye >> x_target >> y_target >> z_target >> x_up >> y_up >> z_up;
	inputFile >> w >> h;
	
	Camera camera = Camera(Vector(x_eye, y_eye, z_eye), Vector(x_target, y_target, z_target), Vector(x_up, y_up, z_up), w, h);
	
	
	// Ray Tracer
	
	int max_depth;
	double ka, kd, ks, alpha;
	inputFile >> max_depth >> ka >> kd >> ks >> alpha;
	
	RayTracer rayTracer = RayTracer(camera, scene, max_depth, ka, kd, ks, alpha);
	
	cout << scene << endl << endl;
	cout << camera << endl << endl;
	cout << rayTracer << endl;
	cout << endl << endl;
	
	
	/////////////////////////////////////////////////
	//// ENREGISTREMENT DE L'IMAGE AU FORMAT BMP ////
	/////////////////////////////////////////////////
	
	cout << "/////////////////////////////////////////////////" << endl;
	cout << "//// ENREGISTREMENT DE L'IMAGE AU FORMAT BMP ////" << endl;
	cout << "/////////////////////////////////////////////////" << endl;
	cout << endl;
	
	cout << "Please wait..." << endl << endl;
	
	
	// Construction de l'image pixel par pixel
	
	int dpi = 72;
	int width = rayTracer.camera.width;
	int height = rayTracer.camera.height;
	int n = width * height;
	
	RGBType* pixels = new RGBType[n];
	
	/* Trièdre associé à la position du plan de l'image DANS LA SCÈNE */
	
	Vector y_direction = rayTracer.camera.up;
	Vector z_direction = rayTracer.camera.eye - rayTracer.camera.target;
	z_direction.normalize();
	Vector x_direction = y_direction.vectorial(z_direction);
	x_direction.normalize();
	
	for (int pixel = 0; pixel < width * height; pixel++) {
		
		int x = pixel % width;       // coordonnée x des pixels SUR L'IMAGE
		int y = (pixel - x) / width; // coordonnée y des pixels SUR L'IMAGE
		
		double x1, y1, z1; // coordonnées réelles des pixels DANS LA SCÈNE
		
		Vector ligne1 = Vector(x_direction.x, y_direction.x, z_direction.x); // ligne 1 de la matrice de changement de base
		Vector ligne2 = Vector(x_direction.y, y_direction.y, z_direction.y); // ligne 2 de la matrice de changement de base
		Vector ligne3 = Vector(x_direction.z, y_direction.z, z_direction.z); // ligne 3 de la matrice de changement de base
		
		x1 = camera.target.x + ligne1 * Vector(x, y, 0);
		y1 = camera.target.y + ligne2 * Vector(x, y, 0);
		z1 = camera.target.z + ligne3 * Vector(x, y, 0);
		
		/* On centre l'image autour du point target */
		
		x1 = x1 - (width / 2) * x_direction.x - (height / 2) * rayTracer.camera.up.x;
		y1 = y1 - (width / 2) * x_direction.y - (height / 2) * rayTracer.camera.up.y;
		z1 = z1 - (width / 2) * x_direction.z - (height / 2) * rayTracer.camera.up.z;
		
		/* On tire un rayon depuis l'oeil de la caméra */
		
		Ray ray = Ray(camera.eye, Vector(x1, y1, z1) - camera.eye);
		
		if (ray.intersects(rayTracer.scene.spheres).first) {
			
			Vector result = rayTracer.recursivePixelCompute(ray, 0);
			
			pixels[pixel].r = result.x;
			pixels[pixel].g = result.y;
			pixels[pixel].b = result.z;
			
		} else {
			
			pixels[pixel].r = 255;
			pixels[pixel].g = 255;
			pixels[pixel].b = 255;
		}
	}
	
	
	// Enregistrement de l'image
	
	savebmp("image_droite.bmp", width, height, dpi, pixels);
	
	delete[] pixels;
	
	cout << "Image rendered successfully." << endl << endl;
	
	return 0;
}
