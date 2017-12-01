#include "Action.cpp"

class Transition;
class State;

/**
 *
 * Clase Transition, representa las transiciones de los
 * estados, tiene un apuntador al estado objetivo
 * (target_state) y a una condicion para checkear
 * si la transicion tiene que llevarse a cabo.
 * Tiene una apuntador a una accion, que debe realizarse cuando
 * la transicion se cumpla, usualmente acciones relacionadas
 * a modificar parametros externos o a encontrar caminos
 *
 **/
class Transition {
public:
    State *target_state;
    Condition *condition;
    Action *action;
    State* get_target_state();
    bool is_triggered();
    void execute_action();
};

/**
 *
 * Clase State, representa los estados de la maquina de
 * estados
 *
 *
 **/
class State {
public:
    Action *action;
    vector <Transition> transitions;
    Action *get_action();
    vector <Transition> get_transitions();
};
