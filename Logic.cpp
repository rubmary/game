#include <cstdlib>
#include "Logic.h"

void Logic::update(double time){
	player -> update(time);
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
	return rand() % graph.get_size();
}

void Logic::appear_coin() {
	coin -> node = choose_position();
	(coin -> character).position = graph.position(coin -> node);
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
	(player_receiver -> character).position = graph.position(player_receiver -> node);
	(agent_receiver  -> character).position = graph.position(agent_receiver  -> node);
}