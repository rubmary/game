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
    Agent agent   = Agent(200, 0, 200, 10);
    Player player = Player(250, 0, 250, 400);        
    
    // Walls
    RectangleShape wall1 = RectangleShape(Vector2f(350, 1));
    RectangleShape wall2 = RectangleShape(Vector2f(150, 1));

    // Rays
    RectangleShape ray  = RectangleShape(Vector2f(30, 1));
    RectangleShape ray1 = RectangleShape(Vector2f(30, 1));
    RectangleShape ray2 = RectangleShape(Vector2f(30, 1));

    void input();
    
    void update(double time);
    
    void draw();

    // The Engine constructor
    Engine(int width, int height, int margin);
};

class EngineKinematic : public Engine {  
    // Kinematics
    KinematicSeek   k_seek;
    KinematicFlee   k_flee;
    KinematicArrive k_arrive;
    KinematicWander k_wander;

public:
    EngineKinematic(int width, int height, int margin);
    // Kinematic's behaviors
    void kinematic_start(double max_speed);

    void kinematic_seek(double time);

    void kinematic_flee(double time);

    void kinematic_arrive(double time);

    void kinematic_wander(double time);

    void start();

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

    void steering_seek(double time);

    void steering_flee(double time);

    void steering_pursue(double time);

    void steering_arrive(double time);

    void steering_wander(double time);

    void start_seek();

    void start_flee();

    void start_pursue();

    void start_arrive();

    void start_wander();
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