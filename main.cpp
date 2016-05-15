#include <iostream>
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Color.h"
#include "Camera.h"

using namespace std;

int main(int argc, const char * argv[]) {
	
	Vector* v = new Vector();
	(*v).normalize();
	
	Vector w = Vector(1, 2, 3);
	cout << "w : " << w << endl;
	w.normalize();
	cout << "w normalized : " << w << endl;
	
	Vector i = Vector(1, 0, 0);
	Vector j = Vector(0, 1, 0);
	Vector k = i.vectorial(j);
	
	cout << "i : " << i << endl;
	cout << "j : " << j << endl;
	cout << "k = i^j = " << k << endl;
	
	Ray r = Ray(Vector(1, 2, 3), k);
	cout << "Et voilà, on a contruit un premier rayon : " << r << endl;
	
	Sphere s = Sphere(Vector(1, 2, 3), 4, Vector(1, 2, 3));
	cout << "Et voilà, on a contruit une première sphère : " << s << endl;
	
	Sphere t = Sphere(Vector(5, 6, 7), 8, Vector(1, 2, 3));
	
	vector<Sphere>* g = new vector<Sphere>();
	(*g).push_back(s);
	(*g).push_back(t);
	cout << "Et voilà, on a contruit une première scène : " << *g << endl;
	
	cout << 2 * i << endl;
	cout << i * i << endl;
	
	Color c = Color(Vector(3.56, 2.6, 24), 0.5);
	cout << c << endl;
	
	Camera cam = Camera();
	cout << cam << endl;
	
	return 0;
}
