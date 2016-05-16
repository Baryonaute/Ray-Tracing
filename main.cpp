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
	
	cout << "Rendering..." << endl;
	
	///////////////////////////////////////
	//// MISE EN PLACE DE L'EXPERIENCE ////
	///////////////////////////////////////
	
	Light light1 = Light(Vector(0, 120, 30));
	Light light2 = Light(Vector(480, 360, 300));
	vector<Light> lights;
	lights.push_back(light1);
	lights.push_back(light2);
	
	Sphere sphere1 = Sphere(Vector(320, 240, 0), 100, Vector(255, 0, 0));
	Sphere sphere2 = Sphere(Vector(420, 340, 0), 100, Vector(255, 0, 122));
	vector<Sphere> spheres;
	spheres.push_back(sphere1);
	//spheres.push_back(sphere2);
	
	Camera camera = Camera(); // Caméra par défaut dans un premier temps
	Scene scene = Scene(spheres, lights);
	RayTracer rayTracer = RayTracer(camera, scene, 0, 0.7, 0.5, 5);
	
	cout << scene << endl;
	cout << camera << endl;
	cout << rayTracer << endl;
	
	/////////////////////////////////////////////////
	//// ENREGISTREMENT DE L'IMAGE AU FORMAT BMP ////
	/////////////////////////////////////////////////
	
	int dpi = 72;
	int width = camera.width;
	int heigth = camera.height;
	int n = width * heigth;
	
	int pixel;
	
	RGBType* pixels = new RGBType[n];
	
	for (int x = 0; x < width; ++x) {
		
		for (int y = 0; y < heigth; ++y) {
			
			pixel = y * width + x;
			
			Ray ray = Ray(camera.eye, Vector(x, y, 0) - camera.eye);
			Vector result2 = Vector(); bool alreadyintersected = false;
			for (vector<spheres>::iterator i = scene.spheres.begin(); i = scene.spheres.end(); ++i) {
				Pair<bool,Vector> intersection = ray.intersect(*i);
				if (intersection.first && !alreadyintersected) {
					alreadyintersected = true;
					result2 = intersection.second;		
									} else if (intersection.first && alreadyintersected){
					Vector temp1 = Vector();
					temp1 = result2 - eye;
					Vector temp2 = intersection.second - eye;
					if(temp2.norm() < temp1.norm()){//mise à jour du champ result2
						result2 = intersection.second;
					}
					
				}
				else {
					
					pixels[pixel].r = 255;
					pixels[pixel].g = 255;
					pixels[pixel].b = 255;
				}
			//Là il est temps de récuperer le résultat de l'intersection et de calculer son pixel
			//On va parler de la partie 3 et du calcul des ombres dans la scene
			Vector result = rayTracer.pixelCompute(ray, sphere1, result2);
					pixels[pixel].r = result.x;
					pixels[pixel].g = result.y;
					pixels[pixel].b = result.z;
			}
		}
	}
	
	savebmp("image.bmp", width, heigth, dpi, pixels);
	
	cout << "Image rendered successfully." << endl;
	
	return 0;
}