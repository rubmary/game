#include "State.h"

State* Transition::get_target_state() {
	return target_state;
}

bool Transition::is_triggered() {
	return condition -> test();
}

Action* State::get_action() {
	return action;
}

vector <Transition *> State::get_transitions(){
	return transitions;
}