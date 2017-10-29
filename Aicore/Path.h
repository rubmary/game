#include <vector>
#include "Collision.cpp"
using namespace std;

class Path {
public:
    Path(vector < Vector3<double> > points);
    vector < Vector3<double> > points;
    double get_param(int segment, Vector3<double> position);
    Vector3<double> get_position(int segment, double param);
    int find_segment(Vector3 <double> position);
};