#include "stdafx.h"
#include "mpi.h"
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

void savebmp(const char *filename, int width, int height, int dpi, RGBType *data) {

	FILE *f;
	int k = width * height;
	int s = 4 * k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi * m;

	unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };

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

		unsigned char color[3] = { blue, green, red };

		fwrite(color, 1, 3, f);
	}

	fclose(f);
}

int main(int argc, char ** argv) {

	std::cout << "Rendering..." << endl;
	//ajout pour MPI
	int my_rank;
	int p;
	int source;
	int dest;
	int tag = 50;
	///////////////////////////////////////
	//// MISE EN PLACE DE L'EXPERIENCE ////
	///////////////////////////////////////

	Light light1 = Light(Vector(100, 200, 500));
	Light light2 = Light(Vector(255, 0, 200));
	vector<Light> lights;
	lights.push_back(light1);
	lights.push_back(light2);

	Sphere sphere1 = Sphere(Vector(320, 500, 0), 100, Vector(10, 255, 10));
	Sphere sphere2 = Sphere(Vector(400, 240, 0), 100, Vector(10, 10, 255));
	Sphere sphere3 = Sphere(Vector(320, 240, 0), 100, Vector(255, 10, 10));
	vector<Sphere> spheres;
	spheres.push_back(sphere1);
	spheres.push_back(sphere2);
	spheres.push_back(sphere3);

	Camera camera = Camera(); // Caméra par défaut dans un premier temps
	Scene scene = Scene(spheres, lights);
	RayTracer rayTracer = RayTracer(camera, scene, 0, 0.7, 0.5, 5);

	//cout << scene << endl;
	//cout << camera << endl;
	//cout << rayTracer << endl;

	/////////////////////////////////////////////////
	//// ENREGISTREMENT DE L'IMAGE AU FORMAT BMP ////
	/////////////////////////////////////////////////

	int dpi = 72;
	int width = camera.width;
	int heigth = camera.height;
	int n = width * heigth;

	int pixel;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);//initialisation de la variable my_rank
	MPI_Comm_size(MPI_COMM_WORLD, &p);//nombre des processus
	if (my_rank != 0) {
		int* pixels = new int[3 * heigth];
		int x = my_rank;
		for (int y = 0; y < heigth; ++y) {
			pixel = y * width + x;
			Ray ray = Ray(camera.eye, Vector(x, y, 0) - camera.eye);
			Vector point = Vector();
			bool alreadyIntersected = false;
			pair<bool, Vector> intersection;
			for (std::vector<Sphere>::iterator i = scene.spheres.begin(); i != scene.spheres.end(); ++i)
			{
				intersection = ray.intersect(*i);
				if (intersection.first && !alreadyIntersected) {
					alreadyIntersected = true;

					point = intersection.second;
					Vector result = rayTracer.pixelCompute(ray, *i, point);
					pixels[3 * pixel] = result.x;
					pixels[3 * pixel + 1] = result.y;
					pixels[3 * pixel + 2] = result.z;

				}
				else if (intersection.first && alreadyIntersected) {
					Vector temp1;
					temp1 = point - camera.eye;
					Vector temp2 = Vector();
					temp2 = intersection.second - camera.eye;
					if (temp2.norm() < temp1.norm()) {
						point = intersection.second; Vector result = rayTracer.pixelCompute(ray, *i, intersection.second);
						pixels[3 * pixel] = result.x;
						pixels[3 * pixel + 1] = result.y;
						pixels[3 * pixel + 2] = result.z;
					}
				}

			}if (!alreadyIntersected) {
				pixels[3 * pixel] = 255;
				pixels[3 * pixel + 1] = 255;
				pixels[3 * pixel + 2] = 255;
			}
		}
		MPI_Send(pixels, 3 * heigth, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}
	if (my_rank == 0) {
		int* pixels; RGBType* resultat = new RGBType[n];
		for (source = 1; source < p; source++) {
			MPI_Recv(pixels, 3 * heigth, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			for (int i = 0; i < heigth; ++i) {
				resultat[p*heigth + i].r = pixels[3 * i];
				resultat[p*heigth + i].g = pixels[3 * i + 1];
				resultat[p*heigth + i].b = pixels[3 * i + 2];
			}
		}
		savebmp("image_MPI.bmp", width, heigth, dpi, resultat);
	}

	//for (int x = 0; x < width; ++x) {
		//if (x == 50) { std::cout << "On rentre dans la boucle" << endl; }

	//}

	MPI_Finalize();

	cout << "Image rendered successfully." << endl;

	return 0;
}
