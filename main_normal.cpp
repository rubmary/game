#include "Engine.cpp"
using namespace sf;

double get_rand(int a, int b) {
	return rand()%b + a;
}

Vector3<double> get_rand_seg(Vector3<double>A, Vector3<double> B) {
	double t = (double) rand() / RAND_MAX;
	return A + (B-A)*t;
}

Vector2f to_vector2(Vector3<double>A) {
	return {(float) A.x, (float) A.z};
}

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

RectangleShape get_shape_point(Vector3<double> P, Color color) {
	Vector2f A = to_vector2(P);
	Vector2f B = {A.x + 2, A.y + 2};
	return get_shape(A, B, color);
}


int main(){
	RenderWindow window;
	window.create(VideoMode(500, 500),
        "Simple Game Engine",
        Style::Default);
	srand(time(NULL));
	

	Vector3<double> A, B, P, n, pos;
	A = {get_rand(50, 400), get_rand(50, 400), get_rand(50, 400)};
	B = {get_rand(50, 400), get_rand(50, 400), get_rand(50, 400)};
	pos = {get_rand(50, 400), get_rand(50, 400), get_rand(50, 400)};
	P = get_rand_seg(A, B);
	n = normal(A, B, P, pos)*15.00;

	RectangleShape segment = get_shape(to_vector2(A), to_vector2(B), Color::Yellow);
	RectangleShape pointP = get_shape_point(P, Color::Red);
	RectangleShape pointPos = get_shape_point(pos, Color::Red);
	RectangleShape segment_normal = get_shape(to_vector2(P), to_vector2(n+P), Color::White);

	window.clear(Color::Black);
	window.draw(segment);
	window.draw(segment_normal);
	window.draw(pointP);
	window.draw(pointPos);
	window.display();

	while(window.isOpen()) {
		if (Keyboard::isKeyPressed(Keyboard::Escape))
        	window.close();
	}
}