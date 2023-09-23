
#include <cmath>


#include "Vector.hpp"




using namespace std;

Vector2D::Vector2D () : m_x(.0), m_y(.0), m_norm(.0), m_angle(.0), m_scale(1.), m_lockRotation(false), m_lockScaling(false)
{}

Vector2D::Vector2D (float x, float y) : m_x(x), m_y(y), m_scale(1.), m_lockRotation(false), m_lockScaling(false)
{
    calculateNorm();
    calculateAngle();
}

// Vector2D::Vector2D (float norm, float angle) : m_norm(norm), m_angle(angle), m_lockRotation(false), m_lockScaling(false)
// {
//     calculateCoords();
// }

void Vector2D::setCoords (float x, float y) {
    m_x = x;
    m_y = y;
    calculateAngle();
    calculateNorm();
}


void Vector2D::calculateNorm() {
    m_norm = sqrtf(m_x * m_x + m_y * m_y);
}

void Vector2D::calculateAngle () {
    m_angle = asin(m_y);
}

void Vector2D::rotateBy (float angle) {
    m_angle += angle;
    calculateCoords();
}

void Vector2D::rotateTo (float angle) {
    m_angle = angle;
    calculateCoords();
}

void Vector2D::calculateCoords () {
    m_x = m_norm * cosf(m_angle);
    m_y = m_norm * sinf(m_angle);
}

float Vector2D::getAngle () const {
    return m_angle;
}

pair<float, float> Vector2D::getCoords () const {
    pair<float, float> c(m_x, m_y);
    return c;
}

void Vector2D::moveBy (float x, float y) {
    m_x += x;
    m_y += y;
    calculateAngle();
    calculateNorm();
}

void Vector2D::moveBy (const Vector2D &vector) {
    m_x += vector.m_x;
    m_y += vector.m_y;
    calculateAngle();
    calculateNorm();
}

void Vector2D::moveTo (float x, float y) {
    m_x = x;
    m_y = y;
    calculateAngle();
    calculateNorm();
}

void Vector2D::moveTo (const Vector2D &vector) {
    m_x = vector.m_x;
    m_y = vector.m_y;
    calculateAngle();
    calculateNorm();
}



void Vector2D::scaleBy (float scale) {
    m_scale *= scale;
    m_norm *= scale;
    calculateCoords();
}

float Vector2D::getNorm () const {
    return m_norm;
}



// statique
Vector2D Vector2D::createVectorFromAngle(float angle) {
    Vector2D v;
    v.m_norm = 1.;
    v.m_angle = angle;
    v.calculateCoords();
    return v;
}

Vector2D& Vector2D::operator+= (Vector2D const &v) {
    moveBy(v);
    return *this;
}


Vector2D operator+ (Vector2D const &v1, Vector2D const &v2) {
    Vector2D res(v1);
    res.moveBy(v2);
    return res;
}

