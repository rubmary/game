#include <cstdlib>
#include "Logic.h"
#define smell_value 100000

void Logic::update(double t){
    *time = t;
    player -> update(*time);
    for (int i = 0; i < agents.size(); i++)
        agents[i] -> update();
    int source = agent_receiver -> node;
    if (agent_receiver -> exists)
        graph -> produce_smell(source, smell_value);
    else
        source = -1;
    graph -> update_smell(agent_receiver -> node, smell_value);
}

void Logic::on_map() {
    *show_map = true;
}

void Logic::off_map() {
    *show_map = false;
}

bool Logic::exists_coin() {
    return coin -> exists;
}

int Logic::choose_position() {
    return rand() % graph -> get_size();
}

void Logic::appear_coin() {
    coin -> node = choose_position();
    (coin -> character).position = graph -> position(coin -> node);
    coin -> exists = true;
    player_receiver -> exists = false;
    agent_receiver  -> exists = false;
}

void Logic::disappear_coin() {
    coin -> exists = false;
    player_receiver -> exists = true;
    agent_receiver  -> exists = true;
    player_receiver -> node = choose_position();
    agent_receiver  -> node = choose_position();
    (player_receiver -> character).position = graph -> position(player_receiver -> node);
    (agent_receiver  -> character).position = graph -> position(agent_receiver  -> node);
}