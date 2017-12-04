#include "Agent.cpp"

/**
 *
 * struct Logic, manejador de toda la
 * logica del juego
 * @player:     apuntador al jugador del juego
 * @coin:       apuntador a la moneda del juego
 * @player_receiver:    receptor del jugador
 * @agent_receiver:     receptor del oponente
 * @agents:             todos los agentes del juego
 * @friends:            amigos del jugador
 * @walls:              paredes del juego
 * @graph:              grafo del juego
 * @show_map:           booleano que indica si
 *                      se debe mostrar o no el mapa
 * @time:               tiempo transcurrido entre cada
 *                      frame
 **/
struct Logic {
    Player* player;
    Object *coin, *player_receiver, *agent_receiver;
    vector <Agent*> agents;
    vector <Friend*> friends;
    vector <Wall> *walls;
    Graph *graph;
    bool* show_map;
    double* time;

    /**
     * update - actualizar logica en un intervalo
     * de tiempo
     * parametro:
     *  time:       intervalo de tiempo tiempo transcurrido
     **/
    void update(double time);

    /**
     * on_map 
     * mostrar todo el mapa
     **/
    void on_map();

    /**
     * off_map 
     * ocultar el mapa
     **/
    void off_map();

    /**
     * exists_coin
     * Consulta si hay una moneda en el
     * mapa o no
     **/
    bool exists_coin();

    /**
     * Elegir un nodo para la posicion de
     * un objeto
     **/
    int choose_position();

    /**
     * appear_coin 
     * hacer aparecer una moneda
     * en una posicion
     **/
    void appear_coin();

    /**
     * disappear_coin
     * hacer desaparecer la moneda
     **/
    void disappear_coin();

    /**
     * set_shadows
     * configura las sombras para una
     * seccion en especifica
     **/
    void set_shadows(int section);

    /**
     * Asigna todos los nodos de sombra
     **/
    void set_shadows();
};

