#include "Aicore/StateMachine.cpp"

/**
 *
 * Clase Agent, representa los
 * objetos con una posicion dinamica
 * en el juego
 *
 **/
class Agent {
public:
    Kinematic character;
    void virtual update();
};

/**
 *
 * Clase Object,
 * Representa los objetos del juego:
 * moneda, punto de recepcion 
 *
 **/
class Object : public Agent{
public:
    int node;
    bool exists;
    void virtual update();
};

/**
 *
 * Clase Character
 * Representa un agente del juego
 * no controlado por el jugador
 *
 **/
class Character : public Agent {
public:
    StateMachine state_machine;
    void virtual update();
};

/**
 *
 * Clase Player - Jugador
 *
 **/
class Player : public Agent {
    bool left_pressed;
    bool right_pressed;
    bool up_pressed;
    bool down_pressed;
public:
    Player(double x, double y, double z, double speed);
    
    /**
     *
     * Mover el jugador en alguna
     * direccion
     **/
    void move_left();
    void move_right();
    void move_up();
    void move_down();

    /**
     *
     * Para el movimiento en alguna
     * direccion
     **/
    void stop_left();
    void stop_right(); 
    void stop_up();
    void stop_down();

    /**
     *
     * Actualizar la posicion del
     * jugador
     **/
    virtual void update(double time);
};