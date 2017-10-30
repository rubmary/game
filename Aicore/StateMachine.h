#include "State.cpp"

/**
 *
 * Clase StateMachine para la maquina de estado
 * tiene un vector de estados, una apuntador al 
 * estado actual y un apuntador al estado inicial
 *
 **/
class StateMachine {
    vector <State> states;
    State *initial_state;
    State *current_state;
public:
    Action* update();
};