#include <vector>
#include "location.cpp"
using namespace std;

struct Collision {
    Vector3 <double> position;
    Vector3 <double> normal;
};

struct Wall {
    Vector3 <double> A, B;
};

class CollisionDetector {
public:
    vector <Wall> *walls;
    int collide(Vector3 <double> position, Vector3 <double> &ray);
    Collision get_collision(Vector3 <double> position, Vector3 <double> ray, int i);
};