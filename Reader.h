#include "DrawableObjects.cpp"

class Reader {
public:
    void read_map(  Graph* &graph,
                    vector <DrawableObject*> &segments,
                    vector<Wall>* &W,
                    vector<Color> colors,
                    bool* &show_map );
    
    void read_agents(   vector <DrawableObject*> &drawable_edges,
                        Player* &player,
                        Object* &coin,
                        Object* &player_receiver,
                        Object* &agent_receiver );

    void read_competitor(   Graph* &graph,
                            Character* &competitor,
                            vector<Wall>* &walls,
                            Player* &player,
                            Object* &coin,
                            double* &time,
                            vector<DrawableObject*> &drawable_objects );
    void read_vigilant( Graph* &graph,
                        Character* &vigilant,
                        // Character* &competitor,
                        vector<Wall>* &walls,
                        Player* &player,
                        Object* &coin,
                        double* &time,
                        vector<DrawableObject*> &drawable_agents);
};