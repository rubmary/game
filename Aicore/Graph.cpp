#include <vector>
#include <queue>
#include <set>
#include <utility>
#include <map>
#include "Graph.h"
using namespace std;



vector<int> Edge::get_differents_nodes(int u){
    if (!valid)
        return vector<int>(0);
    vector <int> diff_nodes;
    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i] != u)
            diff_nodes.push_back(nodes[i]);
    return diff_nodes;
}

bool Triangle::is_inside(Vector3<double> pos) {
    vector <Vector3 <double> > vertices = get_vertices();
    // cout << "is_inside" << endl;
    return inside(vertices[0], vertices[1], vertices[2], pos);
}

vector < Vector3<double> > Triangle::get_vertices() {
    set <pair <double, double> > vertices;
    vertices.insert((a->A));
    vertices.insert((b->A));
    vertices.insert((c->A));
    vertices.insert((a->B));
    vertices.insert((b->B));
    vertices.insert((c->B));
    vector <pair <double, double> > V0(vertices.begin(), vertices.end());
    vector < Vector3<double> > V(3);
    for (int i = 0; i < 3; i++)
        V[i] = {V0[i].first, 0, V0[i].second};
    return V;
}

bool Triangle::not_degenerate() {
    vector < Vector3<double> > V = get_vertices();
    if (collinear(V[0], V[1], V[0], V[2]))
        return false;
    for (int i = 0; i < 3; i++) {
        if (magnitude(V[i] - V[(i+1)%3]) < 0.5)
            return false;
    }
    return true;
}

bool is_triangle(Edge a, Edge b, Edge c) {
    set <pair <double, double> > vertices;
    vertices.insert(a.A);
    vertices.insert(a.B);
    vertices.insert(b.A);
    vertices.insert(b.B);
    vertices.insert(c.A);
    vertices.insert(c.B);
    return vertices.size() == 3;
}

Graph::Graph(vector <Edge>edges) {
    meshes.clear();
    E = edges;
    sort(E.begin(), E.end());
    E.resize(distance(E.begin(), unique(E.begin(), E.end())));
    int M = E.size();
    cout << M << endl;
    n = 0;
    for (int i = 0; i < M; i++) {
        for (int j = i + 1; j < M; j++) {
            for(int k = j + 1; k < M; k++) {
                if (is_triangle(E[i], E[j], E[k])) {
                    Triangle triangle = {&E[i], &E[j], &E[k]};
                    E[i].nodes.push_back(n);
                    E[j].nodes.push_back(n);
                    E[k].nodes.push_back(n);
                    if (triangle.not_degenerate()) {
                        meshes.push_back(triangle);
                        n++;
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++) {
            if (meshes[i] == meshes[j])
                cout << "Los tringulos: " << i << " y " << j << " son iguales" << endl;
        }
    }
}

vector <int> Graph::get_neighbors(int u) {
    vector <int> neighbors, news;
    news = (meshes[u].a) -> get_differents_nodes(u);
    neighbors.insert(neighbors.end(), news.begin(), news.end());
    news = (meshes[u].b) -> get_differents_nodes(u);
    neighbors.insert(neighbors.end(), news.begin(), news.end());
    news = (meshes[u].c) -> get_differents_nodes(u);
    neighbors.insert(neighbors.end(), news.begin(), news.end());
    return neighbors;
}

int Graph::node(int P, Vector3<double> pos) {
    if (meshes[P].is_inside(pos))
        return P;
    vector <int> NB = get_neighbors(P);
    for (int i = 0; i < NB.size(); i++) {
        if (meshes[NB[i]].is_inside(pos))
            return NB[i];
    }
    return P;
}

int Graph::node(Vector3<double> pos) {
    for (int i = 0; i < n; i++)
        if (meshes[i].is_inside(pos))
            return i;
    return -1;
}
void Graph::calculate_positions() {
    positions.resize(n);
    for (int i = 0; i < n; i++) {
        vector <Vector3<double> > vertices = meshes[i].get_vertices();
        positions[i] = {0, 0, 0};
        for (int k = 0; k < 3; k++) {
            positions[i].x += vertices[k].x;
            positions[i].y += vertices[k].y;
            positions[i].z += vertices[k].z;
        }
        positions[i] /= 3.0;
    }
};

Vector3 <double> Graph::position(int i) {
    return positions[i];
}

Path Graph::find_path(Vector3 <double> start, Vector3<double> end) {
    int A = node(start), B = node(end);
    vector <int> nodes = A_star(A, B);
    int len = nodes.size();
    vector < Vector3 <double> > points(len);
    for (int i = 0; i < len; i++)
        points[i] = position(nodes[i]);
    Path path(points);
    return path;
}

double Graph::h(int i, int j) {
    return magnitude(position(i) - position(j));
}


vector<int> Graph::A_star(int start, int end){
    vector <int> parent(n, -1);
    vector <bool> visited(n, false);
    priority_queue <NodeRecord> nodes;
    NodeRecord node_record = {h(start, end), 0, start, -1};
    nodes.push(node_record);
    
    while (!nodes.empty()) {
        NodeRecord current = nodes.top();
        nodes.pop();
        int u = current.node, p = current.parent;
        double cost_so_far = current.cost_so_far;
        if (visited[u])
            continue;
        
        parent[u] = p;
        visited[u] = true;
        if (u == end)
            break;

        vector <int> neighbors = get_neighbors(u);
        for (int i = 0; i < neighbors.size(); i++) {
            int v = neighbors[i];
            double cost, estimated;
            cost = cost_so_far + h(u, v);
            estimated = cost + h(v, end);
            node_record = {estimated, cost, v, u};
            nodes.push(node_record);
        }
    }

    if (end != start && parent[end] == -1)
        return vector <int>(0);

    vector <int> path;
    int current = end;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    return path;
}
int Graph::get_size() {
    return n;
}

Triangle Graph::get_node(int i) {
    cout << "Node: " << i << endl;
    cout << (((meshes[i].a) -> A).first - 4)/2 << ' ' << (((meshes[i].a) -> A).second - 4)/2 << '\t';
    cout << (((meshes[i].a) -> B).first - 4)/2 << ' ' << (((meshes[i].a) -> B).second - 4)/2 << endl;
    cout << (((meshes[i].b) -> A).first - 4)/2 << ' ' << (((meshes[i].b) -> A).second - 4)/2 << '\t';
    cout << (((meshes[i].b) -> B).first - 4)/2 << ' ' << (((meshes[i].b) -> B).second - 4)/2 << endl;
    cout << (((meshes[i].c) -> A).first - 4)/2 << ' ' << (((meshes[i].c) -> A).second - 4)/2 << '\t';
    cout << (((meshes[i].c) -> B).first - 4)/2 << ' ' << (((meshes[i].c) -> B).second - 4)/2 << endl;
    cout << "Centroid: " << (positions[i].x - 4)/2 << ' ' << (positions[i].z - 4)/2 << endl;
    return meshes[i];
}