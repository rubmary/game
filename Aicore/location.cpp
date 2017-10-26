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


// Kinematic's function
void Kinematic::integrate(double t) {
    SIMPLE_INTEGRATION(t, velocity, rotation);
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


void Kinematic::integrate(const SteeringOutput& steer, double drag, double t) {
    SIMPLE_INTEGRATION(t, velocity, rotation);

    drag = pow(drag, t);
    velocity *= drag;
    rotation *= drag;
    velocity += steer.linear*t;
    rotation += steer.angular*t;
}

void Kinematic::integrate(const SteeringOutput& steer, 
                          const SteeringOutput& drag, 
                          double t) {
    SIMPLE_INTEGRATION(t, velocity, rotation);
    velocity.x *= pow(drag.linear.x, t);
    velocity.y *= pow(drag.linear.y, t);
    velocity.z *= pow(drag.linear.z, t);
    rotation   *= pow(drag.angular,  t);

    velocity += steer.linear*t;
    rotation += steer.angular*t;
}
void Kinematic::operator += (const Kinematic& other) {
    position += other.position;
    velocity += other.velocity;
    rotation += other.rotation;
    orientation += other.orientation; 
}

void Kinematic::operator -= (const Kinematic& other) {
    position -= other.position;
    velocity -= other.velocity;
    rotation -= other.rotation;
    orientation -= other.orientation; 
}

void Kinematic::operator *= (double f) {
    position *= f;
    velocity *= f;
    rotation *= f;
    orientation *= f;
}
