#include "steering.h"

void Seek::getSteering(SteeringOutput* output) {
    output -> linear = (*target) - (character -> position);

    if (square_magnitude(output -> linear) > 0)
    {
        normalize(output -> linear);
        output -> linear *= max_acceleration;
    }
}

void Flee::getSteering(SteeringOutput* output) {
    output -> linear = (character -> position) - (*target);
    
    if (square_magnitude(output -> linear) > 0)
    {
        normalize(output -> linear);
        output -> linear *= max_acceleration;
    }
}

void Arrive::getSteering(SteeringOutput* output) {
    Vector3<double> direction = (*target) - (character -> position);
    double distance = square_magnitude(direction);
    double wanted_speed;
    Vector3<double> wanted_velocity;
    if (distance < target_radius) {
        clear_vector(character -> velocity);
        return;
    }
    if (distance <= slow_radius)
        wanted_speed = max_speed*distance/slow_radius;
    else
        wanted_speed = max_speed;
    wanted_velocity = direction;
    normalize(wanted_velocity);
    wanted_velocity *= wanted_speed;

    output -> linear = (wanted_velocity - (character->velocity))/time_to_target;

    if (square_magnitude(output -> linear) > max_acceleration) {
        normalize(output -> linear);
        output -> linear *= max_acceleration;
    }
     
}


void Pursue::getSteering(SteeringOutput* output) {
    Vector3<double> direction = (*pursue_target) - (character -> position);
    double distance = magnitude(direction);
    double speed = magnitude(character -> velocity);
    double prediction;
    // std::cout << max_prediction << std::endl;
    if(speed <= distance / max_prediction)
        prediction = max_prediction;
    else
        prediction = distance / speed;
    *target = direction + (character -> velocity) * prediction;
    Seek::getSteering(output);
    // std::cout << "Estoy en Pursue" <<std::endl;
}


SeekWithInternalTarget::SeekWithInternalTarget() {
    target = &internal_target;
}

void Wander::getSteering(SteeringOutput* output) {
    Vector3<double> base = {1, 0, 1};
    if (square_magnitude((*target))== 0) {
        internal_target  = character->position;
        internal_target += volatility*base;
    }

    Vector3<double> offset = *target - character->position;
    double angle;
    if (offset.x*offset.x + offset.z*offset.z > 0) {
        angle = atan2(offset.z, offset.x);
    }
    else
        angle = 0;
    
    internal_target = character->position;
    internal_target += volatility*(Vector3<double>(cos(angle), 0, sin(angle)));
    
    double change = 1.00*rand()/RAND_MAX - 1.00*rand()/RAND_MAX;

    internal_target += change*base;

    Seek::getSteering(output);
}

void Separation::getSteering(SteeringOutput* output) {
    std::vector<Vector3<double> >::iterator t;
    for (t = targets.begin(); t != targets.end(); t++) {
        Vector3<double> direction = (*t) - (character->position);
        double distance = square_magnitude(direction);
        if (distance < threshold) {
            double strength = std::min(decay_coefficient/(distance*distance),max_acceleration);
            normalize(direction);
            output->linear += (strength * direction);
        }
    }
}

void PrioritySteering::getSteering(SteeringOutput* output) {
    double epSquared = epsilon*epsilon;

    (*output).clear();
    std::vector<SteeringBehaviour*>::iterator b;
    for (b = behaviours.begin(); b != behaviours.end(); b++) {
        (*b)->character = character;
        (*b)->getSteering(output);
        if (square_magnitude((output->linear)) > epSquared) {
            lastUsed = *b;
            return;
        }
    }

}

FollowPath::FollowPath(Path path = vector<Vector3<double> >(0), double path_offset = 0.1)
    : path(path), path_offset(path_offset) {
    segment = 0;
    target = new Vector3<double>{0, 0, 0};
}

void FollowPath::getSteering(SteeringOutput* output) {
    double current_param = path.get_param(segment, character -> position);
    double target_param  = current_param + path_offset;
    if (target_param >= 1) {
        segment++;
        target_param = 0;
    }
    if (segment == path.points.size() - 1) {
        segment--;
        target_param = 1;
    }
    *target = path.get_position(segment, target_param);
    Seek::getSteering(output);
}