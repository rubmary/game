#include "kinematic.cpp"

class SteeringBehaviour {
public:
    Kinematic *character;
    virtual void getSteering(SteeringOutput* output);
};

class Seek : public SteeringBehaviour {
public:
    Vector3<double> *target;
    double max_acceleration;
    virtual void getSteering(SteeringOutput* output);
};

class Arrive : public Seek {
public:
    double max_speed;
    double target_radius;
    double slow_radius;
    double time_to_target;
    virtual void getSteering(SteeringOutput* output);
};

class Pursue : public Seek {
public:
    double max_prediction;
    Vector3<double> *pursue_target;
    virtual void getSteering(SteeringOutput* output);      
};

class Flee : public Seek
{
public:
    virtual void getSteering(SteeringOutput* output);
};

class SeekWithInternalTarget : public Seek {
protected:
    Vector3<double> internal_target;
    SeekWithInternalTarget();
};

class Wander : public SeekWithInternalTarget {
public:
    double volatility;
    double turn_Speed;
    virtual void getSteering(SteeringOutput* output);
};

class Separation : SteeringBehaviour {
public:
    std::vector< Vector3<double> > targets;
    double threshold;
    double decay_coefficient;
    double max_acceleration;
    virtual void getSteering(SteeringOutput* output);
};

class FollowPath : public Seek {
public:
    Path path;
    double path_offset;
    int segment;
    double end_path;
    FollowPath(Path path, double path_offset);
    virtual void getSteering(SteeringOutput* output);
};

class ObstacleAvoidance : public Seek {
public:
    ObstacleAvoidance();
    CollisionDetector collision_detector;
    double lookahead;
    double avoid_distance;
    virtual void getSteering(SteeringOutput* output);
};

class PrioritySteering : public SteeringBehaviour {
public:
    std::vector<SteeringBehaviour*> behaviours;
    double epsilon;
    virtual void getSteering(SteeringOutput* output);
};