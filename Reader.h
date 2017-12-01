#include "DrawableObjects.cpp"

class Reader {
public:
    void read_map(  Graph &graph,
                    vector <DrawableObject*> &segments,
                    vector<Wall> &W,
                    vector<Color> colors,
                    bool* &show_map);
    void read_agents(   vector <DrawableObject*> &drawable_edges,
                        Player* &player,
                        Object* &coin,
                        Object* &player_receiver,
                        Object* &agent_receiver);
};