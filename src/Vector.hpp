

#ifndef VECTOR_H
#define VECTOR_H

#include <utility>


class Vector2D {
    public:

    Vector2D();
    Vector2D(float x, float y);
    // Vector2D(float norm, float angle);

    void setCoords (float x, float y);
    void rotateBy (float angle);
    void rotateTo (float angle);
    void setNorm (float value);
    void moveTo (float x, float y);
    void moveBy (float x, float y);

    void moveBy (const Vector2D &vector);
    void moveTo (const Vector2D &vector);

    // void scaleTo (float scale);
    void scaleBy (float scale);
    
    float getAngle () const;
    float getNorm () const;
    std::pair<float, float> getCoords () const;

    void normalize ();



    static Vector2D createVectorFromAngle (float angle);

    bool equalsTo (Vector2D const &v) const;

    Vector2D& operator+= (Vector2D const &v);
    Vector2D& operator-= (Vector2D const &v);


    protected:
    void calculateNorm ();
    void calculateAngle ();
    void calculateCoords ();




    float m_x;
    float m_y;
    float m_norm;
    float m_angle;
    float m_scale;

    bool m_lockRotation;
    bool m_lockScaling;
    

};

Vector2D operator+ (Vector2D const &v1, Vector2D const &v2);
Vector2D operator- (Vector2D const &v1, Vector2D const &v2);
Vector2D operator* (Vector2D const &v1, float scale);
Vector2D operator/ (Vector2D const &v1, float scale);

bool operator== (Vector2D const &v1, Vector2D const &v2);
bool operator!= (Vector2D const &v1, Vector2D const &v2);




#endif