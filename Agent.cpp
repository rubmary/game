#include "Agent.h"

void Character::update() {
    state_machine.update() -> execute();
    node    = graph -> node(character.position);
    section = graph -> section(node);
}

void Friend::update() {
    state_machine.update() -> execute();
}

Player::Player(double x, double y, double z, double s){
	character.position = {x, y, z};
	character.max_speed = s;
}

void Player::move_left() {
    left_pressed = true;
}

void Player::move_right() {
    right_pressed = true;
}

void Player::move_up() {
    up_pressed = true;
}

void Player::move_down() {
    down_pressed = true;
}

void Player::stop_left() {
    left_pressed = false;
}

void Player::stop_right() {
    right_pressed = false;
}

void Player::stop_up() {
    up_pressed = false;
}

void Player::stop_down() {
    down_pressed = false;
}

void Player::update(double time) {

    if (right_pressed)
        character.position.x += character.max_speed * time;

    if (left_pressed)
        character.position.x -= character.max_speed * time;

    if (up_pressed)
        character.position.z += character.max_speed * time;

    if (down_pressed)
        character.position.z -= character.max_speed * time;    
}