#include <vector>
#include <utility>
#include "Path.cpp"
using namespace std;



struct Edge {
    pair <double, double> A, B;
    bool valid;
    vector <int> nodes;
    vector<int> get_differents_nodes(int u);
};

struct Triangle{
    Edge *a, *b, *c;
    bool is_inside(Vector3 <double> position);
    vector < Vector3<double> > get_vertices();
};

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
    vector <Edge> E;
    vector <Triangle> meshes;
    vector <int> get_neighbors(int u);
    vector <Vector3 <double> > positions;

public:
    Graph(vector <Edge> E);
    Vector3<double> position(int node);
    void calculate_positions();
    int node(int previous, Vector3 <double> position);
    int node(Vector3<double> pos);
    Path find_path(Vector3 <double> start, Vector3<double> end);
    vector<int> A_star(int start, int end);
    double h(int i, int j);
};
