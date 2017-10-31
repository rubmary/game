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

    // ray.setFillColor(Color(150, 50, 250));
    // ray1.setFillColor(Color(150, 50, 250));
    // ray2.setFillColor(Color(150, 50, 250));
    
    // Load the background into the texture
    // Be sure to scale this image to your screen size
    // m_BackgroundTexture.loadFromFile("background.jpg");

    // Associate the sprite with the texture
    // m_BackgroundSprite.setTexture(m_BackgroundTexture);

}

void Engine::start() {
    Clock clock;
    while (m_Window.isOpen()) {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        
        // Make a fraction from the delta time
        double time = dt.asSeconds();

        input();
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
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


void Engine::update(double time) {
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
    m_Window.clear(Color::Black);
    // Draw the background
    m_Window.draw(m_BackgroundSprite);
    m_Window.draw(player.get_sprite());
    m_Window.draw(agent.get_sprite());
    // Show everything we have just drawn
    m_Window.display();
}


// Steering functions
EngineSteering::EngineSteering(int w = 600, int h = 650, int m = 20) { }

void EngineSteering::steering_start(double max_acceleration) { 

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

void EngineTest::start() {
    Clock clock;
    while (m_Window.isOpen())
    {
        // Restart the clock and save the elapsed time into dt
        Time dt = clock.restart();
        time = dt.asSeconds();
        input();
        state_machine.update() -> execute();
        update(time);
        agent.check_bounders(width, height);
        player.check_bounders(width, height);
        draw();
    }
}

void EngineTest::update(double time) {
    Engine::update(time);
    coin.update();
}
void EngineTest::input() {
    Engine::input();
    if (Keyboard::isKeyPressed(Keyboard::M))
    show_map = true;
    
    if (Keyboard::isKeyPressed(Keyboard::N))
        show_map = false;

    if (Keyboard::isKeyPressed(Keyboard::X))
        exist_coin = false;

    if (Keyboard::isKeyPressed(Keyboard::C) && !exist_coin) {
        exist_coin = true;
        coin.character.position.x = random() % 1000 + 50;
        coin.character.position.z = random() % 500 + 50;
    }

}
EngineTest::EngineTest(vector <Wall> W) {
    
    obstacle_avoidance.lookahead      = 30;
    obstacle_avoidance.avoid_distance = 40;
    obstacle_avoidance.collision_detector.walls = W;
    obstacle_avoidance.max_acceleration = 100;
    
    seek.character = &agent.character;
    seek.character -> max_speed = 100;
    seek.target = &player.character.position;
    seek.max_acceleration = 400;
    
    follow_path.character = &agent.character;
    follow_path.segment = 0;
    follow_path.path_offset  = 0.1;
    follow_path.target = new Vector3 <double> {0, 0, 0};
    follow_path.max_acceleration = 400;


    arrive.character = &agent.character;
    arrive.target = &coin.character.position;
    arrive.max_acceleration = 400;
    arrive.max_speed = 200;
    arrive.target_radius = 5;
    arrive.slow_radius = 60;
    arrive.time_to_target = 1;


    priority_steering.behaviours.resize(2);
    priority_steering.behaviours[0] = &follow_path;
    priority_steering.behaviours[1] = &arrive;
    priority_steering.character = &agent.character;
    priority_steering.epsilon = 0.001;

    // action.steering_behavior = &priority_steering;
    // action.time = &time;

    follow_player.steering_behavior = &seek;
    follow_player.time = &time;

    find_coin.steering_behavior = &priority_steering;
    find_coin.time = &time;

    calculate_path.follow_path = &follow_path;
    calculate_path.graph = &graph;
    calculate_path.target = &coin.character.position;
    
    // Inicializando los estados....
    finding_coin.action = &find_coin;
    seeking_player.action = &follow_player;
    finding_coin.transitions.push_back({&seeking_player, &not_coin, &none});
    seeking_player.transitions.push_back({&finding_coin, &check_coin, &calculate_path});

    state_machine.states.push_back(finding_coin);
    state_machine.states.push_back(seeking_player);
    state_machine.initial_state = &state_machine.states[1];
    state_machine.current_state = state_machine.initial_state; 

    check_coin.condition = &exist_coin;
    not_coin.condition = &check_coin;
    coin.sprite.setFillColor(Color::Yellow); 
}

void EngineTest::draw() {
    m_Window.clear(Color::Black);
    m_Window.draw(player.get_sprite());
    m_Window.draw(agent.get_sprite());
    // m_Window.draw(ray1);
    // m_Window.draw(ray);
    // m_Window.draw(ray2);

    
    if (show_map) {
        for (int i = 0; i < map.size(); i++)
            m_Window.draw(map[i]);
        for (int i = 0; i < centroids.size(); i++)
            m_Window.draw(centroids[i]);
    }
    for (int i = 0; i < walls.size(); i++)
        m_Window.draw(walls[i]);

    if (exist_coin)
        m_Window.draw(coin.get_sprite());
    m_Window.display();
}