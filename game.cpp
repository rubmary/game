#include "Engine.cpp"

int main(){
    Reader reader;

    Graph graph;
    vector <Wall> walls;

    vector<Color> colors(4);
    colors[0] = Color(155,64,0);
    colors[1] = Color::Blue;
    colors[2] = Color::Yellow;
    colors[3] = Color::Red;

    vector <DrawableObject*> objects;
    reader.read_map(graph, objects, walls, colors);

    Engine engine(1320, 720, objects, walls, graph);
    engine.start();
}