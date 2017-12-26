#include "DrawableObjects.h"

bool operator < (const Vector2f &A, const Vector2f &B){
    return A.x < B.x || (A.x == B.x && A.y < B.y);
}

RectangleShape make_shape(const Vector2f &A, const Vector2f &B, Color color) {
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


Drawable* DrawableObject::get_shape(){
    return shape;
}

bool DrawableObject::is_visible(){
    return *visible;
}

DrawableWall::DrawableWall(float x1, float y1, float x2, float y2, Color c) {
    color = c;
    shape = new RectangleShape(make_shape({x1, y1}, {x2, y2}, color));
}

DrawablePortal::DrawablePortal(float x, float y, int size, Color c) {
    color = c;
    shape = new CircleShape(size, 6);
    shape -> setFillColor(color);
    visible = new bool(true);
    Vector2f position = {x, y};
    shape -> setPosition(position);
}

bool DrawableWall::is_visible(){
    return true;
}

DrawableEdge::DrawableEdge(float x1, float y1, float x2, float y2, Color c) {
    color = c;
    shape = new RectangleShape(make_shape({x1, y1}, {x2, y2}, color));
}

DrawableAgent::DrawableAgent(Kinematic &chart, Color c, int size) {
    color = c;
    character = &chart;
    shape = new CircleShape(size);
    shape -> setFillColor(color);
    visible = new bool(true);
    get_shape();
}

Shape* DrawableAgent::get_shape() {
    Vector2f position = {(float) (character -> position).x, (float) (character -> position).z};
    shape -> setPosition(position);
    return shape;
}

DrawableText::DrawableText(float x, float y, int size, string txt, Color c, Font &font) {
    color = c;
    text = new Text(txt.c_str(), font);
    text -> setPosition({x, y});
    text -> setCharacterSize(size);
    text -> setColor(color);
    visible = new bool(true);
}

Drawable* DrawableText::get_shape() {
    return text;
}