#include "Action.h"


template <class T>
void SetParamAction <T>::execute() {
    *param = value;
}

template <class T>
void CopyParamAction <T>::execute() {
    *param = *value;
}


void FindBestSegmentAction::execute() {
    follow_path -> segment = (follow_path -> path).find_segment(follow_path -> character -> position);
    follow_path -> end_path = false;
}

void FindBestPath::execute() {
    follow_path -> path = graph -> find_path(follow_path -> character -> position, *target);
    follow_path -> end_path = false;
    follow_path -> segment = 0;
}


void SteeringBehaviorAction::execute() {
    SteeringOutput steering;
    steering_behavior -> getSteering(&steering);
    (steering_behavior -> character) -> integrate(steering, *time);
}

void MultipleActionsAction::execute() {
    for (int i = 0; i < (int) actions.size(); i++)
        actions[i] -> execute();
}