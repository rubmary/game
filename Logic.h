#include "Agent.cpp"

struct Logic {
    Player* player;
    vector <Agent*> agents;
    vector <Wall> walls;
    Graph graph;
    bool* show_map;
    void update(double time);
    void on_map();
    void off_map();
};