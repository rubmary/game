#include "Reader.h"
using namespace std;

void Reader::read_map ( Graph &graph,
                        vector <DrawableObject*> &segments,
                        vector<Wall> &W,
                        vector<Color> colors,
                        bool* &show_map) {
    
    show_map = new bool;
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
            S -> active_visibility = show_map;
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

void Reader::read_agents(vector <Agent*> &agents, 
                         vector <DrawableObject*> &drawable_agents,
                         Player* &player,
                         Object*   &coin) {
    int N;
    ifstream file("Agents.txt");
    file >> N;
    for(int i = 0; i < N; i++) {
        int type;
        file >> type;
        Agent* agent;
        DrawableObject* drawable_agent;
        double x, y, z, speed;
        int size;
        if (type == 0) {
            file >> x >> y >> z >> speed >> size;
            player = new Player(x, y, z, speed);
            drawable_agent = new DrawableAgent(player->character, Color::White, size);
            drawable_agents.push_back(drawable_agent);
        }else if(type == 1) {
            file >> size;
            coin = new Object();
            drawable_agent = new DrawableAgent(coin->character, Color::Yellow, size);
            drawable_agent -> visible = &(coin -> exists);
            drawable_agents.push_back(drawable_agent);
        }
    }
}