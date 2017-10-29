#include "Action.cpp"

class Transition;
class State;

class Transition {
    State *target_state;
    Condition *condition;

public:
    State* get_target_state();
    bool is_triggered();
};

class State {
    Action *action;
    vector <Transition *> transitions;
public:
    Action* get_action();
    vector <Transition *> get_transitions();
};
