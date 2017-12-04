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
    void virtual update() {}
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
    void virtual update(){};
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
    int node, section;
    Graph* graph;
    void virtual update();
};

/**
 *
 * Clase Friend
 * Representan los amigos del
 * jugador
 *
 **/
class Friend : public Character {
public:
    Vector3<double> shadow_point;
    virtual void update();
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
    /**
     *
     * valid_position
     * determina si la posicion es valida
     * lo cual ocurre si no esta demasiado cerca
     * de alguna pared
     *
     **/
    bool valid_position(Vector3<double> position);

    /**
     *
     * Portal de salida
     **/
    int exit_portal(Vector3 <double> position);

public:
    int node, section;
    Graph* graph;
    vector <Vector3 <double> > entry_portals, exit_portals;
    vector <int> permutation;
    
    vector <Wall> walls;
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