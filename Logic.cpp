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
    graph -> update_smell(source, smell_value);
    if(make_appear_coin())
        appear_coin();

    if(make_disappear_coin())
        disappear_coin();
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

bool Logic::make_appear_coin() {
    if (exists_coin())
        return false;

    if (magnitude(  (player -> character).position -
                    (player_receiver -> character).position) < 40)
        return true;
    
    if (magnitude(  (competitor -> character).position -
                    (agent_receiver -> character).position) < 40)
        return true;

    return false;

}

bool Logic::make_disappear_coin() {
    if (!exists_coin())
        return false;    
    if (magnitude(  (player -> character).position -
                    (coin -> character).position) < 10)
        return true;
    if (magnitude(  (competitor -> character).position -
                    (coin -> character).position) < 10)
        return true;
    return false;
}

void Logic::set_shadows(int section){
    vector <Friend*> section_friends;
    for (int i = 0; i < friends.size(); i++){
        if (friends[i] -> section == section)
            section_friends.push_back(friends[i]);
    }
    vector <int> shadows = graph -> get_shadows(section);

    int total = shadows.size();

    for (int i = 0; i < section_friends.size(); i++) {
        if (i%total == 0)
            random_shuffle(shadows.begin(), shadows.end());
        section_friends[i] -> shadow_point = graph -> position(shadows[i%total]);
    }
}

void Logic::set_shadows(){
    for (int i = 0; i < graph -> total_sections(); i++)
        set_shadows(i);
}
