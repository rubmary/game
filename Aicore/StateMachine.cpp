#include "StateMachine.h"
#include <iostream>
using namespace std;

Action* StateMachine::update() {
    cout << "Estoy dentro de la maquina de estados :D " << endl;
    Transition *triggered_transition = NULL;
    vector <Transition> transitions = current_state -> get_transitions();
    for (int i = 0; i < transitions.size(); i++) {
        if (transitions[i].is_triggered()) {
            triggered_transition = &transitions[i];
            break;
        }
    }
    if (triggered_transition) {
        cout << "Se cumplio una transicion, deberia cambiar de estado..." << endl;
        triggered_transition -> execute_action();
        State* target_state = triggered_transition -> get_target_state();
        current_state = target_state;
    }
    return current_state -> get_action();
}
