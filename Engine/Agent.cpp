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

bool Player::valid_position(Vector3<double> position) {
    for (int i = 0; i < walls.size(); i++){
        if (distance_point_segment(walls[i].A, walls[i].B, position) < 10)
            return false;
    }
    return true;
}

int Player::exit_portal(Vector3<double> position) {
    for (int i = 0; i < exit_portals.size(); i++)
        if (magnitude(exit_portals[i] - position) < 10)
            return i;
    return -1;
}

void Player::update(double time) {
    Vector3 <double> new_position = character.position;
    if (right_pressed)
        new_position.x += character.max_speed * time;
    if (left_pressed)
        new_position.x -= character.max_speed * time;
    if (up_pressed)
        new_position.z += character.max_speed * time;
    if (down_pressed)
        new_position.z -= character.max_speed * time;

    if (valid_position(new_position))
        character.position = new_position;

    int portal = exit_portal(character.position);
    if (portal != -1)
        character.position = entry_portals[permutation[portal]];
    node    = graph -> node(character.position);
    section = graph -> section(node);
}