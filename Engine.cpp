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
                Graph graph) : 
                walls(walls), graph(graph){
    
    front.window.create(VideoMode(width, height),
                        "Game",
                        Style::Default);
    front.objects = objects;
}

void Engine::start(){
    while (front.window.isOpen()){
        input();
        front.draw();
    }
}

void Engine::input(){
    if (Keyboard::isKeyPressed(Keyboard::Escape))
        front.window.close();
}