#include "State.cpp"

class StateMachine {
    vector <State> states;
    State *initial_state;
    State *current_state;
public:
    Action* update();
};