#include "steering.cpp"

/**
 *
 * Clase base para las condiciones
 *
 **/
class Condition {
public:
    virtual bool test() = 0;
};


/**
 *
 * Conjuncion de condiciones
 *
 **/
class AndCondition : public Condition {
public:
    Condition *condition1, *condition2;
    bool test();
};

/**
 *
 * Disyuncion de condiciones
 *
 **/
class OrCondition : public Condition {
public:
    Condition *condition1, *condition2;
    bool test();
};

/**
 *
 * Negacion de una condicion
 *
 **/
class NotCondition : public Condition {
public:
    Condition *condition;
    bool test();
};


/**
 *
 * Utilizada para las condiciones:
 * a) Hay moneda
 * f) Termino busqueda de moneda en la seccion
 * h) Hay moneda en la seccion
 * i) Sabe posicion de moneda
 * j) Tengo moneda
 * 
 * Todas estas condiciones dependaran de una variable boolena global
 * (en el juego), que sera modificada por otras funciones 
 *
 **/
class BoolCondition : public Condition {
public:
    bool *condition;
    bool test();
};

/**
 *
 * Utilizada para decidir si un agente tiene
 * mucho tiempo en una seccion, el tiempo
 * debe ser una variable del estado 
 * "Caminar de forma aleatoria" y debe ser
 * decrementado en esta accion
 *
 * b) mucho tiempo en seccion
 *
 **/
class TimerCondition : public Condition {
public:
    int *time;
    bool test();
};

/**
 *
 * Utilizado para decidir si dos variables son
 * iguales, en nuestro juego se utiliza para
 *
 * c) el jugador esta en la misma seccion
 * d) llegar a la seccion
 * l) estar en seccion de vigilancia
 *
 **/

class IntegersMatchCondition : public Condition {
public:
    int *watch1;
    int *watch2;
    bool test();
};

/**
 *
 * Utilizada para decidir si va al estado 2.a o no
 * e) Random
 *
 * Devuelve true o false de forma aleatoria
 *
 **/
class RandomCondition : public Condition {
public:
    int p, q;
    bool test();
};

/**
 *
 * Utilizado para decidir si un agente puede
 * ver su objetivo, condiciones:
 *
 * g) encontrar moneda
 * k) ver el jugador
 *
 **/

class SeeTargetCondition : public Condition {
public:
    Vector3 <double> *target;
    Location *character;
    double *lookahead;
    bool test();
};