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

class EngineSteering : public Engine {  
    // Steerings
public:
    Seek seek;
    Pursue pursue;
    Wander wander;
    Flee flee;
    Arrive arrive;
    Separation separation;
    ObstacleAvoidance obstacle_avoidance;

    EngineSteering(int width, int height, int margin);
    // Kinematic's behaviors
    void steering_start(double max_speed);
};

class EngineFollowPath : public Engine  {
    FollowPath follow_path;
public:
    EngineFollowPath();
    void start();
    void steering_follow_path(double time);
};

class EnginePrioritySteering : public EngineSteering {
    PrioritySteering priority_steering;
public:
    EnginePrioritySteering();
    void start();
    void steering_priority_steering(double time);
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
    State finding_coin;
    State seeking_player;

    // Instancia de la maquina de estado
    StateMachine state_machine;
     
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