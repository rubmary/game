#include "Agent.cpp"

Player::Player(int x = 0, int y = 0, int z = 0, double s = 0)
{
    // How fast does Bob move?
    speed = s;

    // Set the Bob's starting position
    sprite.setFillColor(Color::Green);
    character.position.x = x;
    character.position.y = y;
    character.position.z = z; 
}

void Player::move_left()
{
    left_pressed = true;
}

void Player::move_right()
{
    right_pressed = true;
}

void Player::move_up()
{
    up_pressed = true;
}

void Player::move_down()
{
    down_pressed = true;
}

void Player::stop_left()
{
    left_pressed = false;
}

void Player::stop_right()
{
    right_pressed = false;
}

void Player::stop_up()
{
    up_pressed = false;
}

void Player::stop_down()
{
    down_pressed = false;
}

// Move Player based on the input this frame, 
// the time elapsed, and the speed
void Player::update(double time)
{
    

    if (right_pressed)
    {
        character.position.x += speed * time;
    }

    if (left_pressed)
    {
        character.position.x -= speed * time;
    }

    if (up_pressed)
    {
        character.position.z += speed * time;
    }

    if (down_pressed)
    {
        character.position.z -= speed * time;
    }

    // Now move the sprite to its new position
    set_planes();
    sprite.setPosition(plane_xz);   
    
}
