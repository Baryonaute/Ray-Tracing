#include "mpi.h"
#include <sys/time.h>
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

int main(int argc, char ** argv) {
	
	///////////////////////////////////////
	//// MISE EN PLACE DE L'EXPERIENCE ////
	///////////////////////////////////////

	
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
	
	
	/////////////////////////////////////////////////
	//// ENREGISTREMENT DE L'IMAGE AU FORMAT BMP ////
	/////////////////////////////////////////////////
	
	
	// Construction de l'image pixel par pixel
	
	int dpi = 72;
	int width = camera.width;
	int height = camera.height;
	int n = width * height;

	int pixel;


	// Variables et paramètres MPI

	int my_rank;
	int p;
	int source;
	int tag = 50;

	/*
	MESURE DU TEMPS POUR PC
	clock_t t1, t2;
	t1 = clock();
	*/

	/*
	MESURE DU TEMPS POUR MAC
	*/

	timeval t1, t2;
	gettimeofday(&t1, NULL);

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // initialisation de la variable my_rank
	MPI_Comm_size(MPI_COMM_WORLD, &p); // nombre des processus

	if (my_rank != 0) {

		int step = (width * height) / (p - 1);
		int* pixels = new int[3 * step];
		
		cout << "Process " << my_rank <<" began"<< endl;

		for (int pixel = 0; pixel < step; ++pixel) {

			int x = ((my_rank - 1) * step + pixel) % width;
			int y = (pixel - x) / width + (my_rank - 1) * (height / (p - 1));

			Ray ray = Ray(camera.eye, Vector(x, y, 0) - camera.eye);

			if (ray.intersects(rayTracer.scene.spheres).first) {
				
				Vector result = rayTracer.recursivePixelCompute(ray, 0);
				
				pixels[3 * pixel] = result.x;
				pixels[3 * pixel + 1] = result.y;
				pixels[3 * pixel + 2] = result.z;
				
			} else {
				
				pixels[3 * pixel] = 255;
				pixels[3 * pixel + 1] = 255;
				pixels[3 * pixel + 2] = 255;
			}
		}

		MPI_Send(pixels, 3 * step, MPI_INT, 0, tag, MPI_COMM_WORLD);
		cout << "Process " << my_rank << " ended successfully" << endl;

		delete[] pixels;

		MPI_Finalize();
	}

	if (my_rank == 0) {

		cout << "Main process waiting..." << endl;

		int step = (width * height) / (p - 1);
		int* pixels = new int[3 * step];
		RGBType* resultat = new RGBType[n];

		for (source = 1; source < p; source++) {

			MPI_Recv(pixels, 3 * step, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			cout << "Receiving from " << source << endl;

			for (int i = 0; i < step; ++i) {

				resultat[(source - 1) * n / (p - 1) + i].r = pixels[3 * i];
				resultat[(source - 1) * n / (p - 1) + i].g = pixels[3 * i + 1];
				resultat[(source - 1) * n / (p - 1) + i].b = pixels[3 * i + 2];
			}
		}

		savebmp("image_MPI.bmp", width, height, dpi, resultat);
		cout << "Image rendered successfully" << endl << endl;
		
		delete[] pixels;
		delete[] resultat;

		/*
		MESURE DU TEMPS POUR PC
		t2 = clock();
		float diff = (float)t2 - (float)t1;
		cout << diff << endl;
		*/

		/*
		MESURE DU TEMPS POUR MAC
		*/

		gettimeofday(&t2, NULL);

		double elapsed = (t2.tv_sec - t1.tv_sec) * 1000.0; // sec to ms
		elapsed += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
		cout << "Running time : " << elapsed << " ms" << endl << endl;

		MPI_Finalize();
	}

	return 0;
}
