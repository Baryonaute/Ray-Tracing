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
	
	cout << "///////////////////////////////////////" << endl;
	cout << "//// MISE EN PLACE DE L'EXPERIENCE ////" << endl;
	cout << "///////////////////////////////////////" << endl;
	cout << endl << endl;
	
	
	// Lights
	
	Light light1 = Light(Vector(2560, 990, 140));
	Light light2 = Light(Vector(2560, 930, 140));
	Light light3 = Light(Vector(140, 400, 800));
	vector<Light> lights;
	lights.push_back(light1);
	lights.push_back(light2);
	lights.push_back(light3);
	
	
	// Spheres
	
	Sphere blue = Sphere(Vector(2080, 960, 140), 50, Vector(20, 20, 255), 0.1);
	Sphere red = Sphere(Vector(1280, 960, -400), 400, Vector(255, 20, 20), 0.4);
	Sphere green = Sphere(Vector(480, 960, 0), 400, Vector(20, 255, 20), 0.4);
	vector<Sphere> spheres;
	spheres.push_back(blue);
	spheres.push_back(red);
	spheres.push_back(green);
	
	
	// Superstructure
	
	Camera camera = Camera(); // Caméra par défaut. Deux autres constructeurs permettent des variantes.
	Scene scene = Scene(spheres, lights);
	RayTracer rayTracer = RayTracer(camera, scene, 1, 0.5, 0.6, 8, 200);
	
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
	cout << endl << endl;
	
	cout << "Rendering..." << endl << endl;
	
	
	// Construction de l'image pixel par pixel
	
	int dpi = 72;
	int width = camera.width;
	int height = camera.height;
	int n = width * height;
	
	int pixel;
	
	RGBType* pixels = new RGBType[n];
	
	for (int x = 0; x < width; ++x) {
		
		for (int y = 0; y < height; ++y) {
			
			pixel = y * width + x;
			
			Ray ray = Ray(camera.eye, Vector(x, y, 0) - camera.eye);
			
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
	}
	
	
	// Enregistrement de l'image
	
	savebmp("image.bmp", width, height, dpi, pixels);
	
	cout << "Image rendered successfully." << endl;
	
	return 0;
}
