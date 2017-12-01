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

void Logic::choose_position() {
	coin -> node = rand() % graph.get_size();
}

void Logic::appear_coin() {
	// cout << "Intento aparecer la moneda" << endl;
	choose_position();
	// cout << "elegi el nodo e intento asignar la posicion" << endl;

	(coin -> character).position = graph.position(coin -> node);
	// cout << "Asigne posicion a la moneda" << endl;
	coin -> exists = true;
}

void Logic::disappear_coin() {
	coin -> exists = false;
}