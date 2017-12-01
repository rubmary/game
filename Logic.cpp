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