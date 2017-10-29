#include "StateMachine.h"

Action* StateMachine::update() {
	Transition *triggered_transition = NULL;
	vector <Transition*> transitions = current_state -> get_transitions();
	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i] -> is_triggered()) {
			triggered_transition = transitions[i];
			break;
		}
	}
	if (triggered_transition) {
		State* target_state = triggered_transition -> get_target_state();
		return target_state -> get_action();
	}
}