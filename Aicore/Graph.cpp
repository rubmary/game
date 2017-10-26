#include <vector>
#include <queue>
#include "Graph.h"
using namespace std;


int Graph::node(Vector3<double> position) {
    return 0;
}

Vector3 <double> Graph::position(int node) {
    return {0, 0, 0};
}

Path Graph::find_path(Vector3 <double> start, Vector3<double> end) {
    Path path;
    return path;
}

double Graph::h(int i, int j) {
    return magnitude(position(i) - position(j));
}

vector<int> Graph::A_start(int start, int end){
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

        for (int i = 0; i < E[u].size(); i++) {
            int v = E[u][i];
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