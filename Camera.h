#pragma once
#include "pch.h"


/*
Initializes the variables and creates the key map according to the
given keyboard configuration. On azerty keyboards, 'z', 'q', 's', 'd'
and on qwerty keyboards, 'w', 'a', 's', 'd', allow the user to move
in the 3D world with the left hand while controling the mouse with
the right hand. Indeed, these letters are on the left-hand side of
the keyboard and offer a configuration similar to the arrow pad's one.
*/

class Camera {

public:

    Camera(float x, float y, float z, float psi, float theta, float rotationSpeed, float translationSpeed, int windowWidth, int windowHeight);
    ~Camera() {}

    int   getMouseX() { return m_mouseX; }
    int   getMouseY() { return m_mouseY; }
    float getSightX() { return m_x + sin(m_theta) * sin(m_psi); }
    float getSightY() { return m_y + cos(m_psi); }
    float getSightZ() { return m_z + cos(m_theta) * sin(m_psi); }
    float getX() { return m_x; }
    float getY() { return m_y; }
    float getZ() { return m_z; }

    void setKeyboard(int i, bool state) { m_keys[i] = state; }
    void setMouse(int x, int y) { m_mouseX = x; m_mouseY = y; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    void setZ(float z) { m_z = z; }

    void rotation(int, int);
    void translation(sf::Time dt, sf::Keyboard::Key key);

private:

    enum    DIR { FORWARD, BACKWARD, LEFT, RIGHT };   /* translation direction */
    typedef std::map<DIR, int> k_map;

    k_map    m_keyMap;                               /* associations keys - directions */
    bool     m_keys[255];                             /* keys being pushed (true if pushed) */
    float    m_x;                                     /* sphere center x axis */
    float    m_y;                                     /* sphere center y axis */
    float    m_z;                                     /* sphere center z axis */
    int      m_mouseX;                               /* mouse x position */
    int      m_mouseY;                               /* mouse y position */
    float    m_psi;                                   /* psi angle in spherical coordinates */
    float    m_theta;                                 /* theta angle in spherical coordinates */
    float    m_rotationSpeed;                        /* equivalent to mouse sensibility */
    float    m_translationSpeed;                     /* equivalent to keyboard sensibility */
    float    m_time;                                  /* to keep track of the time between two function calls */

};
