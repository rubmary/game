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

vector <DrawableObject*> Front::make_texts ()
{
    make_numbers();
    vector <DrawableObject*> texts;
    Color c = Color::White;
    font.loadFromFile("FreeMonoBoldOblique.ttf");
    DrawableText *computer, *player, *resist, *resist_computer;
    computer        = new DrawableText(1205, 30,  15, "COMPUTADORA",    c, font);
    player          = new DrawableText(1205, 200, 15, "JUGADOR",        c, font);
    resist          = new DrawableText(1205, 370, 10, "SALUD",          c, font);
    resist_computer = new DrawableText(1205, 540, 10, "SALUD COMPUTADORA", c, font);
    texts.push_back(computer);
    texts.push_back(player);
    texts.push_back(resist);
    texts.push_back(resist_computer);
    return texts;
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

    vector <DrawableObject*> texts = front.make_texts();
    for (int i = 0; i < texts.size(); i++)
        objects.push_back(texts[i]);

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
