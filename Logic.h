#include "Agent.cpp"

struct Logic {
    Player* player;
    vector <Agent*> agents;
    vector <Wall> walls;
    Graph graph;

    void update(double time);
};