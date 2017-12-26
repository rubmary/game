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
    vector <string> numbers;
    Font font;

    
    /**
     *
     * draw,
     * dibuja los objetos que son visibles
     * en el campo
     **/
    void draw();

    /**
     *
     * make_numbers
     * Crea el vector de string de numeros
     * para mostrar los puntajes
     **/
    void make_numbers();

    /**
     *
     * make_text
     * Crea un objeto texto
     **/
    // Text* make_text(float x,
    //                 float y,
    //                 int size,
    //                 string txt,
    //                 Color color);

    /**
     *
     * make_texts
     * Crea los textos de puntaje
     * que seran mostrados en la parte
     * lateral de la pantalla
     **/
    vector <DrawableObject*> make_texts();
};


/**
 *
 * Class Engine
 * Motor del video-juego 
 **/
class Engine{
    Front front;
    Logic logic;
public:
    Engine( int width,
            int height,
            vector<DrawableObject*> objects,
            vector <Wall> *walls,
            Graph *graph,
            Player* player,
            bool* show_map,
            double* time,
            Object* coin,
            Object* player_receiver,
            Object* agent_receiver,
            vector<Agent*> agents,
            vector<Friend*> friends,
            Character *competitor,
            Character *vigilant);
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


