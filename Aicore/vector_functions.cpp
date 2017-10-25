#include "math.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
#define EPS 1e-3
void clear_vector(Vector3<double> &v) {
    v = {0, 0, 0};
}

double square_magnitude(Vector3<double> v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

double magnitude(Vector3<double> v) {
    return sqrt(square_magnitude(v));
}

void normalize(Vector3<double> &v) {
    v /= magnitude(v);
}

double operator ^ (Vector3<double> p, Vector3<double> q) {
	return p.x*q.x + p.z*q.z;
}

double operator * (Vector3<double> p, Vector3<double> q) {
	return p.x*q.z - p.z*q.x;
}

double proj_segment(Vector3<double> a, Vector3<double> b, Vector3<double> c) {
	double r = square_magnitude(a - b);
	if (abs(r) < EPS)
		return 0;
	r = ((c - a)^(b - a))/r;
	r = std::max(r, 0.0);
	r = std::min(r, 1.0);
	Vector3<double> target =  a + (b-a)*r;
	return r;
}

Vector3<double> point_on_segment(Vector3<double> a, Vector3<double> b, double t) {
	return a + (b - a)*t;
}