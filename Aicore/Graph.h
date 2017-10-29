#include <vector>
#include <utility>
#include "Path.cpp"
using namespace std;



struct Edge {
    pair <double, double> A, B;
    bool valid;
    vector <int> nodes;
    vector<int> get_differents_nodes(int u);
    bool operator < (const Edge &e) const {
        Edge e1 = {A, B, valid}, e2 = e;
        if (e1.B < e1.A)
            swap(e1.B, e1.A);
        if (e2.B < e2.A)
            swap(e2.B, e2.A);
        pair < pair <double, double>, pair <double, double> > s1 = {e1.A, e1.B}, s2 = {e2.A, e2.B};
        return s1 < s2;
    }

    bool operator == (const Edge &e) const {
        Edge e1 = {A, B, valid}, e2 = e;
        if (e1.B < e1.A)
            swap(e1.B, e1.A);
        if (e2.B < e2.A)
            swap(e2.B, e2.A);
        pair < pair <double, double>, pair <double, double> > s1 = {e1.A, e1.B}, s2 = {e2.A, e2.B};
        return s1 == s2;
    }
};

struct Triangle{
    Edge *a, *b, *c;
    bool is_inside(Vector3 <double> position);
    vector < Vector3<double> > get_vertices();
    bool not_degenerate();
    bool operator == (Triangle t) {
        vector < Vector3<double> > V1 = get_vertices(), V2 = t.get_vertices();
        vector < pair <double, double> > v1(3), v2(3);
        for (int i = 0; i < 3; i++) {
            v1[i] = {V1[i].x, V1[i].z};
            v2[i] = {V2[i].x, V2[i].z};
        }
        return v1 == v2;
    }
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
    Path find_path(int start, int end);
    vector<int> A_star(int start, int end);
    double h(int i, int j);
    int get_size();
    Triangle get_node(int i);
};
