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

int main(int argc, char * argv[]) {

	///////////////////////////////////////
	//// MISE EN PLACE DE L'EXPERIENCE ////
	///////////////////////////////////////
	/*
	cout << "///////////////////////////////////////" << endl;
	cout << "//// MISE EN PLACE DE L'EXPERIENCE ////" << endl;
	cout << "///////////////////////////////////////" << endl;
	cout << endl << endl;
	*/

	// Lights

	Light light1 = Light(Vector(1280, 495, 70));
	Light light2 = Light(Vector(1280, 465, 70));
	Light light3 = Light(Vector(70, 200, 400));
	vector<Light> lights;
	lights.push_back(light1);
	lights.push_back(light2);
	lights.push_back(light3);


	// Spheres

	Sphere blue = Sphere(Vector(1040, 480, 70), 25, Vector(20, 20, 255), 0.1);
	Sphere red = Sphere(Vector(640, 480, -200), 200, Vector(255, 20, 20), 0.4);
	Sphere green = Sphere(Vector(240, 480, 0), 200, Vector(20, 255, 20), 0.4);
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
	/*
	cout << "/////////////////////////////////////////////////" << endl;
	cout << "//// ENREGISTREMENT DE L'IMAGE AU FORMAT BMP ////" << endl;
	cout << "/////////////////////////////////////////////////" << endl;
	cout << endl << endl;

	cout << "Rendering..." << endl << endl;
	*/

	// Construction de l'image pixel par pixel

	int dpi = 72;
	int width = camera.width;
	int height = camera.height;
	int n = width * height;

	//ajout MPI
	int my_rank;
	int p;
	int source;
	int dest;
	int tag = 50;

	clock_t t1, t2;
	t1 = clock();
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);//initialisation de la variable my_rank
	MPI_Comm_size(MPI_COMM_WORLD, &p);//nombre des processus

	if (my_rank != 0) {

		for (int pixel = 0; pixel < (width*height) / (p - 1); ++pixel) {
			int* pixels = new int[3 * ((width*height) / (p - 1))];
			int x = ((my_rank - 1)*((width*height) / (p - 1)) + pixel) % width;
			int y = (pixel - x) / width;
			y = y + (my_rank - 1)*(height / (p - 1));
			Ray ray = Ray(camera.eye, Vector(x, y, 0) - camera.eye);

			if (ray.intersects(rayTracer.scene.spheres).first) {

				Vector result = rayTracer.recursivePixelCompute(ray, 0);

				pixels[3 * pixel] = result.x;
				pixels[3 * pixel + 1] = result.y;
				pixels[3 * pixel + 2] = result.z;

			}
			else {

				pixels[3 * pixel] = 255;
				pixels[3 * pixel + 1] = 255;
				pixels[3 * pixel + 2] = 255;

			}

			MPI_Send(pixels, 3*(width*height)/(p-1), MPI_INT, 0, tag, MPI_COMM_WORLD);
			delete pixels;

		}
	}

	if (my_rank == 0) {
		cout << "Process 0 start receiving" << endl;
		int* pixels; pixels = new int[3 * (width*height) / (p - 1)]; RGBType* resultat = new RGBType[n];
		for (source = 1; source < p; source++) {
			MPI_Recv(pixels, 3 * (width*height) / (p - 1), MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			cout << "receiving" << source << endl;
			for (int i = 0; i < (width*height) / (p - 1); ++i) {
				resultat[(source - 1)*n / (p - 1) + i].r = pixels[3 * i];
				resultat[(source - 1)*n / (p - 1) + i].g = pixels[3 * i + 1];
				resultat[(source - 1)*n / (p - 1) + i].b = pixels[3 * i + 2];
			}
		}
		savebmp("image_MPI2.bmp", width, height, dpi, resultat);
		cout << "Image rendered successfully." << endl;
		delete[] pixels, resultat;
		t2 = clock();
		float diff = (float)t2 - (float)t1;
		cout << diff << endl;
		MPI_Finalize();

	}
	return 0;
	}