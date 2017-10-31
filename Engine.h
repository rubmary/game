#pragma once
#include "include.h"
using namespace sf;

class Engine {
public:
    
    // A regular RenderWindow
    RenderWindow m_Window;
    // Size 
    int width, height, margin;

    // Declare a sprite and a Texture for the background
    Sprite m_BackgroundSprite;
    Texture m_BackgroundTexture;
    
    // Agents
    Agent agent   = Agent(64, 0, 64, 10);
    Player player = Player(250, 0, 250, 400);        
    // Walls

    // Rays
    RectangleShape ray  = RectangleShape(Vector2f(30, 1));
    RectangleShape ray1 = RectangleShape(Vector2f(30, 1));
    RectangleShape ray2 = RectangleShape(Vector2f(30, 1));

    void start();
    virtual void input();
    
    virtual void update(double time);
    
    virtual void draw();

    // The Engine constructor
    Engine(int width, int height, int margin);
};

class EngineTest : public Engine {
public:
    // Cosas que se dibujan
    vector <RectangleShape> walls;
    vector <RectangleShape> map;
    vector <RectangleShape> centroids;
    vector <RectangleShape> path;
    
    // Instancias de steering behaviors
    ObstacleAvoidance obstacle_avoidance;
    Seek seek;
    FollowPath follow_path;
    Arrive arrive;
    PrioritySteering priority_steering;
    

    // Instancias de condiciones
    BoolCondition check_coin;
    NotCondition not_coin;

    // Instancias de acciones
    SteeringBehaviorAction find_coin;
    SteeringBehaviorAction follow_player;
    FindBestPath calculate_path;
    Action none;
    
    // Instancias de estados -- no lo necesito -- lo dejare temporal :)
    // o tal vez si... no estoy segura todavia de la implementacion jaja
    State finding_coin;
    State seeking_player;

    // Instancia de la maquina de estado
    StateMachine state_machine;

    // Otros atributos del motor
    Agent coin;
    Graph graph;
    bool exist_coin = false;
    double time;
    bool show_map;
    
    EngineTest(vector <Wall> W);
    void draw();
    void input();
    void start();
    void update(double time);
};