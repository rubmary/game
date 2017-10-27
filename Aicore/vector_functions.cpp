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
    return r;
}

Vector3<double> point_on_segment(Vector3<double> a, Vector3<double> b, double t) {
    return a + (b - a)*t;
}

bool parallels (Vector3<double> a, Vector3<double> b,  Vector3<double> c, Vector3<double> d ) {
    return abs((b-a)*(d-c)) < EPS;
}

bool collinear (Vector3<double> a, Vector3<double> b,  Vector3<double> c, Vector3<double> d ) {
    return parallels(a, b, c, d) &&
            abs((a-b)*(a-c)) < EPS && abs((c-d)*(c-a)) < EPS;
}

bool intersect(Vector3<double> a, Vector3<double> b,  Vector3<double> c, Vector3<double> d) {
    if(collinear(a, b, c, d)) {
        if(square_magnitude(a-c) < EPS)
            return true;
        if(square_magnitude(a-d) < EPS)
            return true;
        if(square_magnitude(b-c) < EPS)
            return true;
        if(square_magnitude(b-d) < EPS)
            return true;
        if ( ((c-a)^(c-b))>0 && ((d-a)^(d-b))>0 && ((c-b)^(d-b))>0)
            return false;
        return true;
    }

    return  ((d-a)*(b-a))*((c-a)*(b-a)) <= 0 &&
            ((a-c)*(d-c))*((b-c)*(d-c)) <= 0;
}

Vector3 <double> intersection(Vector3<double> a, Vector3<double> b,  Vector3<double> c, Vector3<double> d) {
    if (square_magnitude(a-c) < EPS)
        return a;
    if (square_magnitude(a-d) < EPS)
        return a;
    if (square_magnitude(b-c) < EPS)
        return b;
    if (square_magnitude(b-c) < EPS)
        return b;

    if (collinear(a, b, c, d)) {
        if (((c-b)^(d-b)) < 0)
            return b;
        if (((c-a)^(d-a)) < 0)
            return a;
        if (((a-d)^(b-d)) < 0)
            return d;
    }

    b = b-a;
    d = c-d;
    c = c-a;

    return a + b * (c*d)/(b*d);
}

Vector3 <double> rotate90(Vector3<double> p) {
    return {-p.z, 0, p.x};
}

bool check_semiplane(Vector3<double> a, Vector3<double> b,  Vector3<double> c, Vector3<double> d) {
    b = b-a;
    c = c-a;
    d = d-a;
    return (d*b)*(c*b) >= 0;
}

Vector3 <double> rotate(Vector3<double> p, double t) {
    return {p.x*cos(t) - p.y*sin(t), 0, p.x*sin(t) + p.y*cos(t)};
}

Vector3 <double> normal(Vector3 <double> A,  Vector3<double> B, Vector3 <double> P, Vector3 <double> position) {
    if (square_magnitude(A - P) < EPS)
        A = B;
    Vector3 <double> n = rotate90(A - P);
    n /= magnitude(n);
    if(!check_semiplane(A, P, position, n))
        n = -n;
    return n;
}

bool inside(Vector3<double> A, Vector3<double> B, Vector3<double> C, Vector3<double> P) {
    if ( ((C-A)*(P-A))*((P-A)*(B-A)) < 0 )
        return false;
    if ( ((C-B)*(P-B))*((P-B)*(A-B)) < 0 )
        return false;
    return true;
}
