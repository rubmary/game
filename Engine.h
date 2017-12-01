#include "Reader.cpp"

/**
 *
 * Struct front, contiene los elementos
 * de la interfaz del motor
 * @window:     ventana del motor
 * @objects:    objetos que se dibujan en el mapa
 **/
struct Front{
    RenderWindow window;
    vector <DrawableObject*> objects;
    
    /**
     *
     * draw,
     * dibuja los objetos que son visibles
     * en el campo
     **/
    void draw();
};


/**
 *
 * Class Engine
 * Motor del video-juego 
 **/
class Engine{
    Front front;
    vector <Wall> walls;
    Graph graph;
public:
    Engine( int width,
            int height,
            vector<DrawableObject*> objects,
            vector <Wall> walls,
            Graph graph);

    /**
     *
     * Iniciar el motor
     **/
    void start();

    /**
     *
     * Manejar el input
     **/
    void input();
};