#include "Agent.cpp"

/**
 *
 * struct Logic, manejador de toda la
 * logica del juego
 * @player:     apuntador al jugador del juego
 * @coin:       apuntador a la moneda del juego
 * @receptor:   apuntador al receptor del juego
 **/
struct Logic {
    Player* player;
    Object *coin, *receptor, *weapon;
    vector <Agent*> agents;
    vector <Wall> walls;
    Graph graph;
    bool* show_map;

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
     * Elegir posicion de la moneda
     **/
    void choose_position();

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




};