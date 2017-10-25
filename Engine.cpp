#include "Engine.h"
#include <iostream>

Engine::Engine(int w = 800, int h = 800, int m = 20)
{
    // Get the screen resolution and create an SFML window and View
    
    width  = w;
    height = h;
    margin = m;

    m_Window.create(VideoMode(width + margin, height + margin),
        "Simple Game Engine",
        Style::Default);
    
    // Load the background into the texture
    // Be sure to scale this image to your screen size
    // m_BackgroundTexture.loadFromFile("background.jpg");

    // Associate the sprite with the texture
    // m_BackgroundSprite.setTexture(m_BackgroundTexture);

}

void Engine::input()
{
    /////////////////////////////////////////////////////////////////////////
    /*
    Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::F5)
            {
                // Capturing the window to a sf::Image
                sf::Image screenshot = m_Window.capture();


                // Making the screenshot name
                std::string screenshotName = "screenshot";

                // Saving to a file
                screenshot.saveToFile("screenshots/" + screenshotName + ".png");
            }
        }

    }
    */
    ////////////////////////////////////////////////////////////////////////


    // Handle the player quitting
    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        m_Window.close();
    }

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
}

void Engine::draw() {
    
    // Rub out the last frame
    m_Window.clear(Color::White);
    // Draw the background
    m_Window.draw(m_BackgroundSprite);
    m_Window.draw(player.get_sprite());
    m_Window.draw(agent.get_sprite());
    
    // Show everything we have just drawn
    m_Window.display();
}


// Kinematic's functions

EngineKinematic::EngineKinematic(int w = 600, int h = 650, int m = 20)
{
}

void EngineKinematic::kinematic_start(double max_speed)
{
    // Kinematic seek
    k_seek.character   = &agent.character;
    k_seek.target      = &player.character.position;
    k_seek.max_speed   = max_speed;

    // Kinematic flee
    k_flee.character   = &agent.character;
    k_flee.target      = &player.character.position;
    k_flee.max_speed   = max_speed;
    
    // Kinematic arrive
    k_arrive.character = &agent.character;
    k_arrive.target    = &player.character.position;
    k_arrive.max_speed = max_speed;
    k_arrive.time_to_target = 3;
    k_arrive.radius = 0.3;

    // Kinematic wander
    k_wander.character = &agent.character;
    k_wander.max_speed = max_speed;
    k_wander.max_rotation = max_speed/4;
}


void EngineKinematic::kinematic_seek(double time)
{
    SteeringOutput steering;
    k_seek.getSteering(&steering);
    agent.character.Location::integrate(steering, time);
}

void EngineKinematic::kinematic_flee(double time)
{
    SteeringOutput steering;
    k_flee.getSteering(&steering);
    agent.character.Location::integrate(steering, time);
}
void EngineKinematic::kinematic_arrive(double time)
{
    SteeringOutput steering;
    k_arrive.getSteering(&steering);
    agent.character.Location::integrate(steering, time);
}

void EngineKinematic::kinematic_wander(double time)
{
    SteeringOutput steering;
    k_wander.getSteering(&steering);
    agent.character.Location::integrate(steering, time);
}

void EngineKinematic::start()
{
    // Timing   
    Clock clock;

    kinematic_start(100);

    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();

        input();
        // kinematic_seek(time);
        kinematic_flee(time);
        // kinematic_arrive(time);
        // kinematic_wander(time);
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
}


// Steering functions
EngineSteering::EngineSteering(int w = 600, int h = 650, int m = 20)
{
}

void EngineSteering::steering_start(double max_acceleration)
{ 
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
}

void EngineSteering::steering_seek(double time)
{
    SteeringOutput steering;
    seek.getSteering(&steering);
    agent.character.integrate(steering, time);
}
void EngineSteering::steering_pursue(double time)
{
    SteeringOutput steering;
    pursue.getSteering(&steering);
    agent.character.integrate(steering, time);
}
void EngineSteering::steering_wander(double time)
{
    SteeringOutput steering;
    wander.getSteering(&steering);
    agent.character.integrate(steering, time);
}
void EngineSteering::steering_flee(double time)
{
    SteeringOutput steering;
    flee.getSteering(&steering);
    agent.character.integrate(steering, time);
}
void EngineSteering::steering_arrive(double time)
{
    SteeringOutput steering;
    arrive.getSteering(&steering);
    agent.character.integrate(steering, time);
}

void EngineSteering::start_seek()
{
    Clock clock;

    steering_start(200);

    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();

        input();
        steering_seek(time);
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
}

void EngineSteering::start_flee()
{
    Clock clock;

    steering_start(700);

    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();

        input();
        steering_flee(time);
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
}

void EngineSteering::start_wander()
{
    Clock clock;

    steering_start(700);

    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();

        input();
        steering_wander(time);
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
}


void EngineSteering::start_pursue()
{
    Clock clock;

    steering_start(700);

    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();
        input();
        steering_pursue(time);
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
}

void EngineSteering::start_arrive()
{
    Clock clock;

    steering_start(700);

    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();

        input();
        steering_arrive(time);
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
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
