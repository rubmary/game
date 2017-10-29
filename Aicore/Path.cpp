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

int Path::find_segment(Vector3 <double> position) {
	int n = points.size();
	int best_segment = -1;
	double best_distance = 1e10;
	for (int i = 0; i < n; i++) {
		double distance = distance_point_segment(points[i], points[i+1], position);
		if (distance < best_distance) {
			best_distance = distance;
			best_segment = i;
		}
	}
	return best_segment;
}