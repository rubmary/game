#include "Logic.cpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <algorithm>
#include <fstream>

bool operator < (const Vector2f &A, const Vector2f &B);
RectangleShape make_shape(const Vector2f &A, const Vector2f &B, Color color);


/**
 *
 * Clase DrawableObjects
 * Clase utilizada para la interfaz
 * del juego
 *
 * @shape:   figura del objeto representado
 * @visible: booleano que indica si el actual objeto 
 *           se debe pintar en el tablero
 **/
class DrawableObject{

protected:
    Shape* shape;
    Color color;
public:
    bool *visible;
    /**
     *
     * Retorna la forma del objeto para
     * ser mostrada en la pantalla
     *
     **/
    virtual Drawable* get_shape();

    /**
     *
     * Indica si el objeto debe ser mostrado
     * en el tablero o no
     *
     **/
    virtual bool is_visible();
};

/**
 *
 * Clase DrawableWall
 * Clase utilizada para representar la interfaz
 * de las paredes de la clase
 *
 **/
class DrawableWall : public DrawableObject {
public:
    DrawableWall(float x1, float y1, float x2, float y2, Color color);
    virtual bool is_visible();
};

/**
 *
 * Clase DrawableEdge
 * Clase utilizada para representar la interfaz
 * de los edges del grafo
 *
 **/
class DrawableEdge : public DrawableObject {
public:
    DrawableEdge(float x1, float y1, float x2, float y2, Color color);
};

/**
 *
 * Clase DrawableAgent
 * Clase utilizada para representar la interfaz
 * de los agentes
 *
 **/
class DrawableAgent : public DrawableObject {
public:
    DrawableAgent(Kinematic &character, Color color, int size);
    Kinematic* character;
    virtual Shape* get_shape();
};

class DrawablePortal : public DrawableObject {
public:
    DrawablePortal(float x1, float y1, int size, Color color);
};

class DrawableText : public DrawableObject {
public:
    Text* text;
    DrawableText(float x, float y, int size, string txt, Color color, Font &font);
    virtual Drawable* get_shape();
};

class DrawableNumber : public DrawableText {
public:
    int *score;
    vector <string> *numbers;
    DrawableNumber(
        float x,
        float y,
        int size,
        string txt,
        Color c,
        Font &font,
        vector <string> &numbers,
        int &score);
    virtual Drawable* get_shape();
};