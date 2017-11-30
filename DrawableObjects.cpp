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


Shape* DrawableObject::get_shape(){
	return shape;
}

bool DrawableObject::is_visible(){
	return visible;
}

void DrawableObject::make_visible(){
	visible = true;
}

void DrawableObject::make_invisible(){
	visible = false;
}


DrawableWall::DrawableWall(float x1, float y1, float x2, float y2, Color c) {
	color = c;
	shape = new RectangleShape(make_shape({x1, y1}, {x2, y2}, color));
}

bool DrawableWall::is_visible(){
	return true;
}

DrawableEdge::DrawableEdge(float x1, float y1, float x2, float y2, Color c) {
	color = c;
	shape = new RectangleShape(make_shape({x1, y1}, {x2, y2}, color));
}

bool DrawableEdge::is_visible(){
    if (active_visibility == NULL)
        return true;
	return *active_visibility;
}

DrawableAgent::DrawableAgent(Kinematic &chart, Color c, int size) {
    color = c;
    character = &chart;
    shape = new CircleShape(size);
    shape -> setFillColor(color);
    get_shape();
}

Shape* DrawableAgent::get_shape(){
    Vector2f position = {(float) (character -> position).x, (float) (character -> position).z};
    shape -> setPosition(position);
    return shape;
}