#include "Condition.h"
#include <cstdlib>

bool AndCondition::test() {
    return (condition1 -> test()) && (condition2 -> test()); 
}

bool OrCondition::test() {
    return (condition1 -> test()) || (condition2 -> test()); 
}

bool NotCondition::test() {
    return !(condition -> test());
}

bool BoolCondition::test() {
    return *condition;
}

bool RandomCondition::test() {
    return rand()%q < p;
}

bool TimerCondition::test() {
    return *time < 0;
}

bool IntegersMatchCondition::test() {
    return *watch1 == *watch2;
}

bool SeeTargetCondition::test() {
    if (magnitude(*target - (character -> position)) > *lookahead)
        return false;
    return true;
    Vector3 <double> orientation, A, B, C;
    orientation = character -> get_orientation_as_vector();
    B = character -> position;
    orientation = character -> velocity;
    A = rotate(orientation, -45) + B;
    C = rotate(orientation,  45) + B;
    return inside_arch(A, B, C, *target);
}