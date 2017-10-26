#include "Graph.cpp"

class KinematicMovement
{
public:
	Location *character;
	double max_speed;

	virtual void getSteering(SteeringOutput* output) const = 0;	
};

class TargetedKinematicMovement : public KinematicMovement
{
public:
	Vector3<double> *target;
};

class KinematicSeek : public TargetedKinematicMovement
{
public:
	virtual void getSteering(SteeringOutput* output) const;
};


class KinematicFlee : public TargetedKinematicMovement
{
public:
	virtual void getSteering(SteeringOutput* output) const;
};

class KinematicArrive : public TargetedKinematicMovement
{
public:
	double time_to_target;
	double radius;

	virtual void getSteering(SteeringOutput* output) const;
};

class KinematicWander : public KinematicMovement
{
public:
	double max_rotation;
	virtual void getSteering(SteeringOutput* output) const;
};