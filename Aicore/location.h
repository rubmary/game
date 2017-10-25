#include "vector_functions.cpp"
#include <SFML/Graphics.hpp>

using namespace sf;

// double square_magnitude(Vector3 <double> v);
// double magnitude(Vector3 <double> v);
// void clear_vector(Vector3 <double> &v);
// void normalize(Vector3<double> &v);

struct SteeringOutput
{   

    Vector3 <double> linear;
    double angular;

    void clear()
    {
        clear_vector(linear);
        angular = 0;
    }

    bool operator == (const SteeringOutput& other) const { 
        return linear == other.linear && angular == other.angular;
    }
    bool operator != (const SteeringOutput& other) const {
        return linear != other.linear || angular != other.angular;
    }

};

struct Location{
    
    Vector3<double> position;
    double orientation;
    double max_speed;

    void clear()
    {
        clear_vector(position);
        orientation = 0;
    }

    void set_orientation_from_velocity(const Vector3<double> velocity);    
    Vector3<double> get_orientation_as_vector();

    void integrate(const SteeringOutput& steer, double duration);
};

struct Kinematic : public Location
{
    Vector3 <double> velocity;
    double rotation;

    void clear()
    {
        Location::clear();
        clear_vector(velocity);
        rotation = 0;
    }

    Kinematic& operator = (const Location& other)
    {
        orientation = other.orientation;
        position = other.position;
        return *this;
    }

    Kinematic& operator = (const Kinematic& other)
    {
        orientation = other.orientation;
        position = other.position;
        velocity = other.velocity;
        rotation = other.rotation;
        return *this;
    }

    void integrate(double t);
    void integrate(const SteeringOutput& steer, double t);
    void integrate(const SteeringOutput& steer, double drag, double t);
    void integrate(const SteeringOutput& steer,
                   const SteeringOutput& drag,
                   double t);

    void operator += (const Kinematic&);
    void operator -= (const Kinematic&);
    void operator *= (double f);

};

