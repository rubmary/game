#include "Agent.h"
#include <iostream>

Agent::Agent(int x = 0, int y = 0, int z = 0, double s = 0)
{
    
    
    speed = s;
    character.position.x = x;
    character.position.y = y;
    character.position.z = z;
    sprite.setFillColor(Color::White);
}

void Agent::check_bounders(int width, int height)
{
    if (character.position.x > width)
    {
        character.position.x = width;
    }
    if (character.position.z > height)
    {
        character.position.z = height;
    }
    if (character.position.x < 0)
    {
        character.position.x = 0;
    }
    if (character.position.z < 0)
    {
        character.position.z = 0;
    }
}

void Agent::set_planes()
{
    plane_xz.x = character.position.x;
    plane_xz.y = character.position.z;
    plane_xy.x = character.position.x;
    plane_xy.y = character.position.y;
}

// Make the private spite available to the draw() function
CircleShape Agent::get_sprite()
{
    return sprite;
}


// Move Bob based on the input this frame, 
// the time elapsed, and the speed
void Agent::update()
{
    // Now move the sprite to its new position
    set_planes();
    sprite.setPosition(plane_xz);
}