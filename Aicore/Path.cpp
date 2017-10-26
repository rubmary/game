#include <vector>
#include "Path.h"
using namespace std;

Path::Path(vector<Vector3 <double> >points = vector<Vector3 <double> >(0)) : points(points) {
}

double Path::get_param(int segment, Vector3<double> position) {
    return proj_segment(points[segment], points[segment+1], position);
}

Vector3<double> Path::get_position(int segment, double param){
    return point_on_segment(points[segment], points[segment+1], param);
}