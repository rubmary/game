#include "Engine.h"
#include <iostream>

Engine::Engine(int w = 1300, int h =700, int m = 20) {
// Get the screen resolution and create an SFML window and View
    width  = w;
    height = h;
    margin = m;

    m_Window.create(VideoMode(width + margin, height + margin),
        "Simple Game Engine",
        Style::Default);

    wall1.setPosition(150, 250);
    wall2.setPosition(150, 400);
    wall1.setFillColor(Color(150, 50, 250));
    // wall2.setFillColor(Color(150, 50, 250));
    ray.setFillColor(Color(150, 50, 250));
    ray1.setFillColor(Color(150, 50, 250));
    ray2.setFillColor(Color(150, 50, 250));
    
    // Load the background into the texture
    // Be sure to scale this image to your screen size
    // m_BackgroundTexture.loadFromFile("background.jpg");

    // Associate the sprite with the texture
    // m_BackgroundSprite.setTexture(m_BackgroundTexture);

}

void Engine::input() {
// Handle the player quitting
    if (Keyboard::isKeyPressed(Keyboard::Escape))
        m_Window.close();

    // Handle the player moving
    if (Keyboard::isKeyPressed(Keyboard::A))
        player.move_left();
    else
        player.stop_left();
    
    if (Keyboard::isKeyPressed(Keyboard::D))
        player.move_right();
    else
        player.stop_right();                       
    
    if (Keyboard::isKeyPressed(Keyboard::S))
        player.move_up();
    else
        player.stop_up();                       
    
    if (Keyboard::isKeyPressed(Keyboard::W))
        player.move_down();
    else
        player.stop_down();
}

void Engine::update(double time)
{
    player.update(time);
    agent.update();
    ray.setPosition(agent.character.position.x, agent.character.position.z);
    ray1.setPosition(agent.character.position.x, agent.character.position.z);
    ray2.setPosition(agent.character.position.x, agent.character.position.z);
    double ang = atan2(agent.character.velocity.z, agent.character.velocity.x);
    ang = ang*90/acos(0.0);
    ang  = ang < 0 ? ang + 360 : ang;
    ray.setRotation(ang);
    ray1.setRotation(ang+20);
    ray2.setRotation(ang-20);
}

void Engine::draw() {
    
    // Rub out the last frame
    m_Window.clear(Color::White);
    // Draw the background
    m_Window.draw(m_BackgroundSprite);
    m_Window.draw(player.get_sprite());
    // m_Window.draw(agent.get_sprite());
    m_Window.draw(wall1);
    m_Window.draw(wall2);
    m_Window.draw(ray);
    m_Window.draw(ray1);
    m_Window.draw(ray2);
    // Show everything we have just drawn
    m_Window.display();
}


// Steering functions
EngineSteering::EngineSteering(int w = 600, int h = 650, int m = 20) { }

void EngineSteering::steering_start(double max_acceleration) { 
    seek.character = &agent.character;
    seek.character -> max_speed = 100;
    seek.target = &player.character.position;
    seek.max_acceleration = max_acceleration;
    
    pursue.character = &agent.character;
    pursue.pursue_target = &player.character.position;
    pursue.max_prediction = 100;

    wander.character = &agent.character;
    wander.target = &player.character.position;
    wander.max_acceleration = max_acceleration;
    wander.volatility = 5;
    
    flee.character = &agent.character;
    flee.target = &player.character.position;
    flee.max_acceleration = max_acceleration;
    
    arrive.character = &agent.character;
    arrive.target = &player.character.position;
    arrive.max_acceleration = max_acceleration;
    arrive.max_speed = 250;
    arrive.target_radius = 15;
    arrive.slow_radius = 60;
    arrive.time_to_target = 1;

    obstacle_avoidance.lookahead = 40;
    obstacle_avoidance.avoid_distance = 40;
    obstacle_avoidance.collision_detector.walls.resize(1);
    obstacle_avoidance.collision_detector.walls[0] = {{150, 0, 250}, {500, 0, 250}};
    // obstacle_avoidance.collision_detector.walls[1] = {{415, 0, 235}, {415, 0, 265}};
    // obstacle_avoidance.collision_detector.walls[2] = {{415, 0, 265}, {235, 0, 265}};
    // obstacle_avoidance.collision_detector.walls[3] = {{235, 0, 265}, {235, 0, 235}};
    // obstacle_avoidance.collision_detector.walls[1] = {{200, 0, 400}, {430, 0, 400}};
    obstacle_avoidance.max_acceleration = max_acceleration;
}



EngineFollowPath::EngineFollowPath() { }
void EngineFollowPath::steering_follow_path(double time) {
    SteeringOutput steering;
    follow_path.getSteering(&steering);
    agent.character.integrate(steering, time);
}

void EngineFollowPath::start() {
    Clock clock;
    follow_path.path.points.resize(5);
    follow_path.path.points[0] = {250, 0, 250};
    follow_path.path.points[1] = {400, 0, 250};
    follow_path.path.points[2] = {400, 0, 400};
    follow_path.path.points[3] = {250, 0, 400};
    follow_path.path.points[4] = {250, 0, 550};
    follow_path.character = &agent.character;
    follow_path.max_acceleration = 500;
    agent.character.max_speed = 100;
    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();

        input();
        steering_follow_path(time);
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
}

void EnginePrioritySteering::steering_priority_steering(double time) {
    SteeringOutput steering;
    priority_steering.getSteering(&steering);
    agent.character.integrate(steering, time);
}
EnginePrioritySteering::EnginePrioritySteering() { }
void EnginePrioritySteering::start() {
    Clock clock;

    steering_start(300);
    priority_steering.behaviours.resize(2);
    priority_steering.behaviours[0] = &obstacle_avoidance;
    priority_steering.behaviours[1] = &seek;
    priority_steering.character = &agent.character;
    priority_steering.epsilon = 0.001;
    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();

        input();
        steering_priority_steering(time);
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
}
