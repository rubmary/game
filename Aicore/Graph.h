#include <vector>
#include <utility>
#include "Path.cpp"
using namespace std;



struct Edge {
    pair <double, double> A, B;
    bool valid;
    vector <int> nodes;
    int type;
    vector<int> get_differents_nodes(int u);
    bool operator < (const Edge &e) const {
        Edge e1 = {A, B, valid}, e2 = e;
        if (e1.B < e1.A)
            swap(e1.B, e1.A);
        if (e2.B < e2.A)
            swap(e2.B, e2.A);
        pair < pair <double, double>, pair <double, double> > s1 = {e1.A, e1.B}, s2 = {e2.A, e2.B};
        return s1 < s2;
    }

    bool operator == (const Edge &e) const {
        Edge e1 = {A, B, valid}, e2 = e;
        if (e1.B < e1.A)
            swap(e1.B, e1.A);
        if (e2.B < e2.A)
            swap(e2.B, e2.A);
        pair < pair <double, double>, pair <double, double> > s1 = {e1.A, e1.B}, s2 = {e2.A, e2.B};
        return s1 == s2;
    }
};

struct Triangle{
    Edge *a, *b, *c;
    bool is_inside(Vector3 <double> position);
    vector < Vector3<double> > get_vertices();
    bool not_degenerate();
    bool operator == (Triangle t) {
        vector < Vector3<double> > V1 = get_vertices(), V2 = t.get_vertices();
        vector < pair <double, double> > v1(3), v2(3);
        for (int i = 0; i < 3; i++) {
            v1[i] = {V1[i].x, V1[i].z};
            v2[i] = {V2[i].x, V2[i].z};
        }
        return v1 == v2;
    }
    int smell;
    int section;
};

struct NodeRecord {
    double estimated_total_cost;
    double cost_so_far;
    int node, parent;
    bool operator < (NodeRecord v)const {
        if (estimated_total_cost != estimated_total_cost)
            return estimated_total_cost > v.estimated_total_cost;
        if (cost_so_far != v.cost_so_far)
            return cost_so_far > v.cost_so_far;
        return node < v.node;
    }
};

/**
 *
 * Clase Graph:
 * Representacion del mapa del juego
 * @n:          Numero de nodos
 * @E:          edges (arcos) del grafo
 * @meshes:     cada triangulo del grafo
 * @positions:  las posiciones de los centroides
 *              de los nodos del grafo
 **/
class Graph{
    int n;
    vector <Edge> E;
    vector <Triangle> meshes;
    vector <Vector3 <double> > positions;

    /**
     *
     * get_neighbors
     * retorna los nodos adyacentes a un
     * nodo especifico
     **/
    vector <int> get_neighbors(int u);

public:
    /**
     *
     * Constructor de la clase:
     * construye el grafo a partir de los arcos
     **/
    Graph(vector <Edge> E);

    /**
     *
     * calculate_positions - calcular posiciones
     * Hace el preprocesamiento necesario para
     * luece el preprocesamiento necesario para
     * lugo retornar las posiciones en O(1) en
     * tiempo
     **/
    void calculate_positions();

    /**
     *
     * position - posicion
     * retorna la posicion de un nodo dado
     **/
    Vector3<double> position(int node);

    /**
     *
     * node
     * Devuelve el identificador del nodo
     * correspondiente a la posicion dada
     **/
    int node(Vector3<double> pos);

    /**
     *
     * node
     * Devuelve el identificador del nodo
     * correspondiente a la posicion dada,
     * sabiendo el nodo previo
     **/
    int node(int previous, Vector3 <double> position);

    /**
     *
     * find_path
     * encuentra el camino mas corto entre dos puntos
     * del mapa
     **/
    Path find_path(Vector3 <double> start, Vector3<double> end);

    /**
     *
     * find_path
     * encuentra el camino mas corto entre dos puntos
     * del mapa, conociendo los nodos correspondientes
     **/
    Path find_path(int start, int end);

    /**
     *
     * A_star
     * aplica el algoritmo de A* para encontrar
     * el camino mas corto
     **/
    vector<int> A_star(int start, int end);

    /**
     *
     * h - euristica
     * euristica utilizada en el
     * algoritmo de A*
     **/
    double h(int i, int j);

    /**
     *
     * get_size
     * retorna la cantidad de nodos del grafo
     **/
    int get_size();

    /**
     *
     * Retorna el triangulo correspondiente
     * a un nodo
     **/
    Triangle get_node(int i);

    /**
     *
     * reset_smell
     * quita todos los olores del campo
     **/
    void reset_smell();

    /**
     *
     * produce_smell
     * Produce olor en un nodo en especifico
     **/
    void produce_smell(int node, int val);

    /**
     *
     * update_smell
     * propaga el olor en el campo
     **/
    void update_smell(int source, int val);

    /**
     *
     * random_movement
     * elegir un nuevo nodo de forma aleatoria
     * entre el nodo actual y los nodos vecinos
     **/
    int random_movement(int current_node);

    /**
     *
     * follow_smell
     **/
    int follow_smell(int current_node);

    /**
     *
     * find_edge
     * encuentra el arco comun en dos
     * nodos vecino, retorna null si los
     * nodos no son vecinos
     **/
    Edge* find_edge(int u, int v);

    /**
     *
     * valid_edge
     * determina si el arco entre dos
     * nodos es valido para las secciones
     **/
    bool valid_edge(int u, int v);

    /**
     *
     * dfs
     * dfs para encontrar las secciones del
     * grafo
     **/
    void dfs(int node, int section);

    /**
     *
     * calculate_sections
     * Calcula las secciones del mapa
     * calculando las componentes conexas del
     * mapa usando dfs
     **/
    void calculate_sections();

};
