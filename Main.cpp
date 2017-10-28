#include "Engine.cpp"
using namespace sf;

bool operator < (const Vector2f &A, const Vector2f &B){
    return A.x < B.x || (A.x == B.x && A.y < B.y);
}

RectangleShape get_shape(const Vector2f &A, const Vector2f &B, Color color) {
    Vector2f v = B - A;
    if (B < A)
        v = -v;
    float norma = sqrt(v.x*v.x + v.y*v.y);
    double angle = atan2(v.y, v.x)*90/acos(0.0);
    RectangleShape shape = RectangleShape(Vector2f(norma, 2));
    if (B < A)
        shape.setPosition(B);
    else
        shape.setPosition(A);
    shape.setRotation(angle);
    shape.setFillColor(color);
    return shape;
}
int main() {

    
    double width = 1210, height = 710;
    int N, ratio;
    cin >> N >> ratio;
    vector <Color> color(4);
    vector <RectangleShape> shape(N);
    color[0] = Color(155,64,0);
    color[1] = Color::Blue;
    color[2] = Color::Yellow;
    color[3] = Color::Red;
    vector <Edge> edges(N);

    int x1, y1, x2, y2, c;
    for (int i = 0; i < N; i++) {
        cin >> x1 >> y1 >> x2 >> y2 >> c;
        // if (c == 1) continue;
        x1 = x1*ratio + 4;
        y1 = y1*ratio + 4;
        x2 = x2*ratio + 4;
        y2 = y2*ratio + 4;
        edges[i] = {{x1, y1}, {x2, y2}, c == 1 || c == 2, vector <int>(0)};
        shape[i] = get_shape({ (float) x1,  (float) y1}, { (float) x2,  (float) y2}, color[c]);
    }

    cout << "before make graph..." << endl;

    Graph graph(edges);
    int n = graph.get_size();
    cout << "before calculate positions..." << endl;
    graph.calculate_positions();
    cout << n << endl;

    vector <RectangleShape> centroids(n);
    for (int i = 0; i < n; i++) {
        Vector3 <double> G = graph.position(i);
        centroids[i] = get_shape({(float)G.x, (float)G.z}, {(float)G.x + 2, (float) G.z}, Color::White);
    }

    cout << "hola :D" << endl;
    String title = "Mapa, triangulacion";
    RenderWindow window(VideoMode(width, height), title);

    window.clear(Color::Black);
    
    for (int i = 0; i < N; i++)
        window.draw(shape[i]);
    
    for (int i = 0; i < n; i++)
        window.draw(centroids[i]);

    while (window.isOpen()) {
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();
        window.display();
    }

    return 0;
}
