#include "StateMachine.cpp"
#include <SFML/Graphics.hpp>

using namespace sf;

struct Agent
{    
    // Constructor for Agent
    Agent(int x, int y, int z, double s);

    // Where is the agent
    Kinematic character;
    Vector2f plane_xz;
    Vector2f plane_xy;

    // sprite
    CircleShape sprite = CircleShape(10, 6);

    double speed;

    // Send a copy of the sprite to main
    CircleShape get_sprite();
    
    // Public functions
    void set_texture(int width, int height);
    void set_planes();
    void check_bounders(int width, int height);
       
    // We will call this function once every frame
    virtual void update();
};

struct Player : public Agent
{

    // Constructor for Player
    Player(int x, int y, int z, double s); 

    // Which direction(s) is the player currently moving in
    
    bool left_pressed;
    bool right_pressed;
    bool up_pressed;
    bool down_pressed;

    // Move Bob in a specific direction
    void move_left();

    void move_right();
    
    void move_up();

    void move_down();
    
    // Stop Bob moving in a specific direction
    void stop_left();

    void stop_right();
    
    void stop_up();

    void stop_down();
   
    // We will call this function once every frame
    void update(double time);
};

