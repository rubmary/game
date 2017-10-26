#include <vector>
#include "Path.cpp"
using namespace std;


struct NodeRecord {
    double estimated_total_cost;
    double cost_so_far;
    int node, parent;
    bool operator < (NodeRecord v)const {
        if (estimated_total_cost != estimated_total_cost)
            return estimated_total_cost > v.estimated_total_cost;
        if (cost_so_far != v.cost_so_far)
            return cost_so_far > v.cost_so_far;
        return node < v.node;
    }
};
class Graph{
    int n;
    vector <Vector3 <double> > positions;
    vector <vector <int> > E;

public:
    Vector3 <double> position(int node);
    int node(Vector3 <double> position);
    Path find_path(Vector3 <double> start, Vector3<double> end);
    vector<int> A_start(int start, int end);
    double h(int i, int j);
};
