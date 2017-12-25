#include "Engine.h"
using namespace std;
using namespace sf;

void Front::draw()
{
    window.clear(Color::Black);

    for (int i = 0; i < objects.size(); i++) {
        if(objects[i] -> is_visible())
            window.draw(*(objects[i] -> get_shape()));
    }
}

void Front::make_numbers()
{
    numbers.resize(100);
    for (int i = 0; i < 100; i++)
        numbers[i] = to_string(i);
    for (int i = 0; i < 10; i++)
        numbers[i] = "0" + numbers[i];
}

Text* Front::make_text( float x,
                        float y,
                        int size,
                        string txt,
                        Color color)
{
    Font font;
    font.loadFromFile("FreeMonoBoldOblique.ttf");
    Text* text = new Text(txt.c_str(), font);
    text -> setPosition({x, y});
    text -> setCharacterSize(size);
    text -> setColor(color);
    return text;
}

Engine::Engine( int width,
                int height,
                vector<DrawableObject*> objects,
                vector <Wall> *walls,
                Graph* graph,
                Player* player,
                bool* show_map,
                double* time,
                Object* coin,
                Object* player_receiver,
                Object* agent_receiver,
                vector<Agent*> agents,
                vector<Friend*> friends,
                Character *competitor,
                Character *vigilant)
{
    
    logic.walls  = walls;
    logic.graph  = graph;
    logic.player = player;
    logic.show_map = show_map;
    logic.time = time;
    logic.coin = coin;
    logic.player_receiver = player_receiver;
    logic.agent_receiver  = agent_receiver;
    logic.agents = agents;
    logic.friends = friends;
    logic.competitor = competitor;
    logic.vigilant   = vigilant;
    logic.coin_keeper = -1;
    logic.player_points = 0;
    logic.competitor_points = 0;
    logic.graph -> calculate_positions();
    logic.graph -> reset_smell();
    logic.graph -> calculate_sections();
    logic.set_shadows();
    front.make_numbers();
    front.window.create(VideoMode(width, height),
                        "Game",
                        Style::Default);
    front.objects = objects;
}

void Engine::start(){

    Clock clock;
    logic.appear_coin();
    logic.show_points();

    
    Font font;
    font.loadFromFile("FreeMonoBoldOblique.ttf");
    Text    computer("COMPUTADORA", font),
            player("JUGADOR", font),
            resist("SALUD JUGADOR", font),
            resist_computer("SALUD COMPUTADORA", font);


    computer.setPosition({1205, 30});
    computer.setColor(Color::White);
    computer.setCharacterSize(15);
    player.setPosition({1205, 200});
    player.setColor(Color::White);
    player.setCharacterSize(15);
    resist.setPosition({1205, 370});
    resist.setColor(Color::White);
    resist.setCharacterSize(10);
    resist_computer.setPosition({1205, 540});
    resist_computer.setColor(Color::White);
    resist_computer.setCharacterSize(10);

    string p = "00", c = "00", r = "30", rc = "30";
    Text    player_points(p.c_str(), font),
            computer_points(c.c_str(), font),
            resist_value(r.c_str(), font),
            resist_comp_valu(rc.c_str(), font);

    computer_points.setPosition({1205, 50});
    computer_points.setColor(Color::White);
    computer_points.setCharacterSize(70);
    player_points.setPosition({1205, 220});
    player_points.setColor(Color::White);
    player_points.setCharacterSize(70);
    resist_value.setPosition({1205, 390});
    resist_value.setColor(Color::White);
    resist_value.setCharacterSize(70);
    resist_comp_valu.setPosition({1205, 560});
    resist_comp_valu.setCharacterSize(70);
    resist_comp_valu.setColor(Color::White);


    while (front.window.isOpen() && !logic.finish_game()){
        Time dt = clock.restart();
        double time = dt.asSeconds();
        input();
        logic.update(time);
        front.draw();
        front.window.draw(computer);
        front.window.draw(player);
        front.window.draw(resist);
        front.window.draw(resist_computer);
        computer_points.setString(front.numbers[logic.competitor_points].c_str());
        player_points.setString(front.numbers[logic.player_points].c_str());
        resist_value.setString(front.numbers[logic.player_lifes].c_str());
        resist_comp_valu.setString(front.numbers[logic.competitor_lifes].c_str());
        front.window.draw(computer_points);
        front.window.draw(player_points);
        front.window.draw(resist_value);
        front.window.draw(resist_comp_valu);
        front.window.display();
    }
    while (front.window.isOpen())
        if(Keyboard::isKeyPressed(Keyboard::Escape))
            front.window.close();
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
        logic.on_map();

    if (Keyboard::isKeyPressed(Keyboard::N))
        logic.off_map();
}
