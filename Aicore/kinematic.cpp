#include "kinematic.h"

void KinematicSeek::getSteering(SteeringOutput* output) const
{
    output -> linear = (*target) - (character -> position);

    if (square_magnitude(output -> linear) > 0)
    {
        normalize(output -> linear);
        output -> linear *= max_speed;
    }
}

void KinematicFlee::getSteering(SteeringOutput* output) const
{
    output -> linear = (character -> position) - (*target);
    
    if (square_magnitude(output -> linear) > 0)
    {
        normalize(output -> linear);
        output -> linear *= max_speed;
    }
}

void KinematicArrive::getSteering(SteeringOutput* output) const
{
    output -> linear = *target - (character->position);

    if (square_magnitude(output -> linear) < radius*radius)
    {
        clear_vector(output->linear);
    }
    else
    {
        output -> linear /= time_to_target;
        if (square_magnitude(output -> linear) > max_speed*max_speed)
        {
            normalize(output -> linear);
            output -> linear *= max_speed;
        }
    }
}

void KinematicWander::getSteering(SteeringOutput* output) const
{
    output -> linear = character -> get_orientation_as_vector();
    output -> linear *= max_speed;

    double change = 1.00*rand()/RAND_MAX - 1.00*rand()/RAND_MAX;
    output->angular = change * max_rotation;
}