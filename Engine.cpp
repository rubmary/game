#include "Engine.h"
using namespace std;
using namespace sf;

void Front::draw(){
    window.clear(Color::Black);

    for (int i = 0; i < objects.size(); i++) {
        if(objects[i] -> is_visible())
            window.draw(*(objects[i] -> get_shape()));
    }

    window.display();
}

Engine::Engine( int width,
                int height,
                vector<DrawableObject*> objects,
                vector <Wall> walls,
                Graph graph,
                Player* player,
                bool* show_map) {
    logic.walls  = walls;
    logic.graph  = graph;
    logic.player = player;
    logic.show_map = show_map;
    front.window.create(VideoMode(width, height),
                        "Game",
                        Style::Default);
    front.objects = objects;
}

void Engine::start(){

    Clock clock;
    while (front.window.isOpen()){
        Time dt = clock.restart();
        double time = dt.asSeconds();
        input();
        logic.update(time);
        front.draw();
    }
}

void Engine::input(){
    if (Keyboard::isKeyPressed(Keyboard::Escape))
        front.window.close();

    if (Keyboard::isKeyPressed(Keyboard::A))
        logic.player -> move_left();
    else
        logic.player -> stop_left();
    
    if (Keyboard::isKeyPressed(Keyboard::D))
        logic.player -> move_right();
    else
        logic.player -> stop_right();                       
    
    if (Keyboard::isKeyPressed(Keyboard::S))
        logic.player -> move_up();
    else
        logic.player -> stop_up();                       

    if (Keyboard::isKeyPressed(Keyboard::W))
        logic.player -> move_down();
    else
        logic.player -> stop_down();

    if (Keyboard::isKeyPressed(Keyboard::M))
        *(logic.show_map) = true;

    if (Keyboard::isKeyPressed(Keyboard::N))
        *(logic.show_map) = false;
}
