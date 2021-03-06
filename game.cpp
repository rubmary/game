#include "Engine/Engine.cpp"
using namespace std;

int main(){
    Reader reader;
    srand(time(NULL));
    

    vector<Color> colors(4);
    colors[0] = Color(155,64,0);
    colors[1] = Color(25, 0, 51);
    colors[2] = Color::Yellow;
    colors[3] = Color::Red;

    Graph *graph;
    vector <Wall> *walls, player_walls;
    vector <DrawableObject*> objects, drawable_agents;
    vector <Agent*> agents;
    vector <Friend*> friends;
    Player* player;
    Character *competitor, *vigilant;
    Object *coin, *player_receiver, *agent_receiver;
    bool* show_map;
    double* time;

    reader.read_map(graph, objects, walls, player_walls, colors, show_map);
    reader.read_agents(graph, drawable_agents, player, coin, player_receiver, agent_receiver, player_walls);
    reader.read_competitor(graph, competitor, walls, player, coin, time, drawable_agents);
    reader.read_vigilant(graph, vigilant, walls, player, coin, time, drawable_agents);
    reader.read_friends(graph, competitor, walls, time, friends, drawable_agents);
    agents.push_back(competitor);
    agents.push_back(vigilant);
    
    for (int i = 0; i < friends.size(); i++)
        agents.push_back(friends[i]);

    for (int i = 0; i < drawable_agents.size(); i++)
        objects.push_back(drawable_agents[i]);

    Engine engine(1320, 720, objects, walls, graph,
                    player, show_map, time, coin,
                    player_receiver, agent_receiver,
                    agents, friends, competitor, vigilant);
    engine.start();
    return 0;
}