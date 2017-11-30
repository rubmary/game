#include "DrawableObjects.cpp"

class Reader {
public:
    void read_map(  Graph &graph,
                    vector <DrawableObject*> &segments,
                    vector<Wall> &W,
                    vector<Color> colors);
};