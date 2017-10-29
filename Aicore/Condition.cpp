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
    return rand()%2;
}

bool TimerCondition::test() {
    return *time < 0;
}

bool IntegersMatchCondition::test() {
    return *watch1 == *watch2;
}
