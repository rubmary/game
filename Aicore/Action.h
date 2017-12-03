#include <vector>
#include "Condition.cpp"
using namespace std;

/**
 *
 * Clase base para las acciones
 *
 **/
class Action {
public:
    virtual void execute() {}
};

/**
 *
 * Cambia una variable por un valor especifico
 *
 **/
template <class T>
class SetParamAction : public Action{
public:
    T value;
    T *param;
    void execute();
};

/**
 *
 * Copia el valor de una variable a otra
 *
 **/
template <class T>
class CopyParamAction : public Action {
public:
    T *value;
    T *param;
    void execute();
};

/**
 *
 * Apunta a un steering behavior del tipo
 * FollowPath y calcula el segmento mas
 * cercano al caracter y modifica 'segment'
 * en la instancia de FollowPath a la que
 * apunta
 *
 **/
class FindBestSegmentAction : public Action {
public:
    FollowPath *follow_path;
    void execute();
};

/**
 *
 * Apunta a un steering behavior del tipo
 * FollowPath y tiene un target en especifico
 * calcula el mejor camino del caracter al
 * target y lo modifica en el steering behavior
 *
 **/
class FindBestPath : public Action {
public:
    Vector3 <double> *target;
    // int *start, *end;
    FollowPath *follow_path;
    Graph *graph;
    void execute();
};

/**
 *
 * Acciones que utilizan steering behavior
 * utilizadas en los estados
 *
 **/
class SteeringBehaviorAction : public Action {
public:
    SteeringBehaviour *steering_behavior;
    double *time;
    virtual void execute();
};


class FindNode : public Action {
public:
    Seek *seek;
    int *node;
    Graph *graph;
    void execute();
};


/**
 *
 * Accion para seguir el olor
 **/
class FollowSmell : public SteeringBehaviorAction {
public:
    Graph *graph;
    Seek *seek;
    int *node;
    virtual void execute();
};

/**
 *
 * Accion para estar aleatoriamente
 * en una zona
 *
 **/
class RandomMovement : public SteeringBehaviorAction {
public:
    Graph *graph;
    Seek  *seek;
    int   *node;
    virtual void execute();
};

/**
 *
 * Realiza multiples acciones
 **/
class MultipleActionsAction : public Action {
public:
    vector <Action*> actions;
    void execute();
};