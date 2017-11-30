#include "Reader.h"
using namespace std;

void Reader::read_map ( Graph &graph,
						vector <DrawableObject*> &segments,
						vector<Wall> &W,
						vector<Color> colors) {
	
	int N, ratio;
	ifstream file("MAP.txt");
	file >> N >> ratio;
	int x1, y1, x2, y2, c;
	segments.clear();
	vector <Edge> edges(N);
	for (int i = 0; i < N; i++) {
		file >> x1 >> y1 >> x2 >> y2 >> c;
		x1 = x1*ratio + 4;
        y1 = y1*ratio + 4;
        x2 = x2*ratio + 4;
        y2 = y2*ratio + 4;
        edges[i] = {{x1, y1}, {x2, y2}, c == 1 || c == 2, vector <int>(0), c};
        if (c == 1) {
        	DrawableEdge *S =  new DrawableEdge(x1, y1, x2, y2, colors[c]);
            segments.push_back(S);
        } else {
        	DrawableWall *S =  new DrawableWall(x1, y1, x2, y2, colors[c]);
            segments.push_back(S);
        }
	}
	graph = Graph(edges);
    for (int i = 0; i < edges.size(); i++) {
        if (!edges[i].valid)
            W.push_back({ {edges[i].A.first, 0, edges[i].A.second},
                          {edges[i].B.first, 0, edges[i].B.second} });
    }
	file.close();
}