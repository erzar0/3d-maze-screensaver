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
    sf::Vector3f getPosition() { return sf::Vector3f(m_x, m_y, m_z); }
    float getPsi() { return m_psi; }
    float getTheta() { return m_theta; }
    float getXYRotationAngle() { return m_xy_rotation_angle; }
    float getFov() { return m_fov; }
    float isPerspectiveProjection() { return m_perspective_projection; }

    void setMouse(int x, int y) { m_mouseX = x; m_mouseY = y; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    void setZ(float z) { m_z = z; }
    void setPosition(const sf::Vector3f& newPosition) { m_x = newPosition.x; m_y = newPosition.y; m_z = newPosition.z; }
    void setPsi(float psi) { m_psi = psi; }
    void setXYRotationAngle(float xy_rot) { m_xy_rotation_angle = fmodf(xy_rot, (float) (std::numbers::pi * 2.0)); }
    void setTheta(float theta) { m_theta = theta; }
    void setFov(float fov) { m_fov = fov; }
    void setPerspectiveProjection(bool perspectiveProjection) { m_perspective_projection = perspectiveProjection; }

    void rotation(int, int);
    void translation(sf::Time dt, sf::Keyboard::Key key);

private:

    float    m_x;                                 
    float    m_y;                                  
    float    m_z;                                   
    float    m_psi;                                  
    float    m_theta;                                
    float    m_xy_rotation_angle = (float) std::numbers::pi / 2.0f;
	bool     m_perspective_projection{ true };
	float    m_fov{ 90.0f };
    int      m_mouseX;                              
    int      m_mouseY;                             
    float    m_rotationSpeed;                     
    float    m_translationSpeed;                 
    float    m_time;
};
