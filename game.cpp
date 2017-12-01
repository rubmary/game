#include "Engine.cpp"
using namespace std;

int main(){
    Reader reader;

    

    vector<Color> colors(4);
    colors[0] = Color(155,64,0);
    colors[1] = Color::Blue;
    colors[2] = Color::Yellow;
    colors[3] = Color::Red;

    Graph graph;
    vector <Wall> walls;
    vector <DrawableObject*> objects, drawable_agents;
    vector <Agent*> agents;
    Player* player;
    bool* show_map;

    reader.read_map(graph, objects, walls, colors, show_map);
    cout << "Lei el mapa" << endl;
    reader.read_agents(agents, drawable_agents, player);
    
    for (int i = 0; i < drawable_agents.size(); i++)
        objects.push_back(drawable_agents[i]);
    cout << "Lei los archivos" << endl;
    cout << "no se que pasa" << endl;
    Engine engine(1320, 720, objects, walls, graph, player, show_map);    
    
    cout << "Cree el engine" << endl;
    engine.start();
}