#include "location.h"
#include <iostream>

// Location's function

#define PI 2*acos(0.0)
#define SIMPLE_INTEGRATION(t, velocity, rotation) \
    position += (velocity)*(t);  \
    orientation += (rotation)*t; \
    orientation = fmod(orientation + PI, 2*PI) - PI;

void Location::integrate(const SteeringOutput& steer, double t) {
    SIMPLE_INTEGRATION(t, steer.linear, steer.angular);
}

void Location::set_orientation_from_velocity(const Vector3<double> velocity) {
    // Check if we have a velocity
    if (square_magnitude(velocity) > 0) {
        orientation = atan2(velocity.x, velocity.z);
    }
}

Vector3<double> Location::get_orientation_as_vector() {
    return {sin(orientation), 0, cos(orientation)};   
}



void Kinematic::integrate(const SteeringOutput& steer, double t)
{
    SIMPLE_INTEGRATION(t, velocity, rotation);
    velocity += steer.linear*t;
    if (magnitude(velocity) > max_speed){
        normalize(velocity);
        velocity *= max_speed;
    }
    rotation += steer.angular*t;
}