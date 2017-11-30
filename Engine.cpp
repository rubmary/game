#include "Engine.h"
#include <iostream>

Engine::Engine(int width = 1300, int height =700, int margin = 20) :
    width(width), height(height), margin(margin) {

    window.create(VideoMode(width + margin, height + margin),
        "Simple Game Engine",
        Style::Default);
}

void Engine::start() {
    Clock clock;
    while (window.isOpen()) {
        Time dt = clock.restart();
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
        window.close();

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
}

void Engine::draw() {
    window.clear(Color::Black);
    window.draw(m_BackgroundSprite);
    window.draw(player.get_sprite());
    window.draw(agent.get_sprite());
    window.display();
}


EngineTest::EngineTest(vector <Wall> W) {
    
    // Inicialinzando evitacion de paredes, no funciona :(
    obstacle_avoidance.lookahead      = 20;
    obstacle_avoidance.avoid_distance = 20;
    obstacle_avoidance.collision_detector.walls = W;
    obstacle_avoidance.max_acceleration = 500;
    
    // Inicializando seek
    seek.character = &agent.character;
    seek.character -> max_speed = 100;
    seek.target = &player.character.position;
    seek.max_acceleration = 400;
    
    // inicializando follow_path
    follow_path.character = &agent.character;
    follow_path.segment = 0;
    follow_path.path_offset  = 0.1;
    follow_path.target = new Vector3 <double> {0, 0, 0};
    follow_path.max_acceleration = 400;

    // inicializando arrive
    arrive.character = &agent.character;
    arrive.target = &coin.character.position;
    arrive.max_acceleration = 400;
    arrive.max_speed = 200;
    arrive.target_radius = 5;
    arrive.slow_radius = 60;
    arrive.time_to_target = 1;

    // inicializando priority_steerin
    priority_steering.behaviours.resize(3);
    priority_steering.behaviours[0] = &obstacle_avoidance;
    priority_steering.behaviours[1] = &follow_path;
    priority_steering.behaviours[2] = &arrive;
    priority_steering.character = &agent.character;
    priority_steering.epsilon = 0.001;

    //inicializando priority_steering2
    priority_steering2.behaviours.resize(2);
    priority_steering2.behaviours[0] = &obstacle_avoidance;
    priority_steering2.behaviours[1] = &seek;
    priority_steering2.character = &agent.character;
    priority_steering2.epsilon = 0.001;


    // inicializando las condiciones de la maquina de estado
    check_coin.condition = &exist_coin;
    not_coin.condition = &check_coin;

    // inicializando las acciones de la maquina de estados
    follow_player.steering_behavior = &priority_steering2;
    follow_player.time = &time;

    find_coin.steering_behavior = &priority_steering;
    find_coin.time = &time;

    calculate_path.follow_path = &follow_path;
    calculate_path.graph = &graph;
    calculate_path.target = &coin.character.position;
    
    // Inicializando los estados de la maquina de estados
    finding_coin.action = &find_coin;
    finding_coin.transitions.push_back({&seeking_player, &not_coin, &none});
    seeking_player.action = &follow_player;
    seeking_player.transitions.push_back({&finding_coin, &check_coin, &calculate_path});

    // inicializando la maquina de estados
    state_machine.states.push_back(finding_coin);
    state_machine.states.push_back(seeking_player);
    state_machine.initial_state = &state_machine.states[1];
    state_machine.current_state = state_machine.initial_state; 

    // cambiando la modena para que parezca moneda
    coin.sprite.setFillColor(Color::Yellow);

}

void EngineTest::start() {
    Clock clock;
    while (window.isOpen()) {
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
        coin.character.position.x = random() % 1100 + 50;
        coin.character.position.z = random() % 650 + 50;
    }
}


void EngineTest::draw() {
    window.clear(Color::Black);
    window.draw(player.get_sprite());
    window.draw(agent.get_sprite());
    
    if (show_map) {
        for (int i = 0; i < map.size(); i++)
            window.draw(map[i]);
        for (int i = 0; i < centroids.size(); i++)
            window.draw(centroids[i]);
    }
    for (int i = 0; i < walls.size(); i++)
        window.draw(walls[i]);

    if (exist_coin)
        window.draw(coin.get_sprite());
    window.display();
}