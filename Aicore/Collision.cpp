#include <vector>
#include "Collision.h"
using namespace std;

int CollisionDetector::collide(Vector3 <double> position, Vector3 <double> &ray) {
    int n = walls.size();
    double alpha = 20.00*acos(0.0)/90;
    Vector3 <double> ray1 = rotate(ray, alpha), ray2 = rotate(ray, -alpha);
    for (int i = 0; i < n; i++) {
        
        if (intersect(walls[i].A, walls[i].B, position, position+ray1)) {
            ray = ray1;
            return i;
        }

        if (intersect(walls[i].A, walls[i].B, position, position+ray))
            return i;
        
        if (intersect(walls[i].A, walls[i].B, position, position+ray2)) {
            ray = ray2;
            return i;
        }
    }
    return -1;
}
Collision CollisionDetector::get_collision(Vector3 <double> position, Vector3 <double> ray, int i) {
    Collision collision;
    collision.position = intersection(walls[i].A, walls[i].B, position, position+ray);
    collision.normal   = normal(walls[i].A, walls[i].B, collision.position, position);
    return collision;
}