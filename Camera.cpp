#pragma once
#include "pch.h"
#include "Camera.h"


Camera::Camera(float x, float y, float z, float psi, float theta, float rotationSpeed, float translationSpeed, int windowWidth, int windowHeight) :
    m_x(x),
    m_y(y),
    m_z(z),
    m_mouseX(windowWidth / 2),
    m_mouseY(windowHeight / 2),
    m_psi(psi),
    m_theta(theta),
    m_rotationSpeed(rotationSpeed),
    m_translationSpeed(translationSpeed),
    m_time(0) 
{
    for (int i = 0; i < 255; i++) { m_keys[i] = false; }
	m_keyMap[FORWARD] = static_cast<int>('w');
	m_keyMap[BACKWARD] = static_cast<int>('s');
	m_keyMap[LEFT] = static_cast<int>('a');
	m_keyMap[RIGHT] = static_cast<int>('d');
}


/*
Computes the new angle values given the mouse direction. The sight's
only limits are when looking up or down: it is impossible for the user to
look at his feets or straight up in the air (these edge cases would need
a special treatment in the gluLookAt function).
*/
void Camera::rotation(int x, int y) {
    std::cout << x;
    m_theta -= static_cast<float>(x - m_mouseX) * m_rotationSpeed;
    m_psi += static_cast<float>(y - m_mouseY) * m_rotationSpeed;
    if (m_psi <= 0.1) m_psi = 0.1;
    else if (m_psi >= 0.95f * (float) std::numbers::pi) m_psi = 0.95f * (float) std::numbers::pi;
    m_mouseX = x;
    m_mouseY = y;
}

/*
Computes the new sphere center given the speed and direction of the camera.
The direction depends on the current angles values, the keys being pushed,
and the elapsed time since the last function call.
*/
void Camera::translation(sf::Time dt, sf::Keyboard::Key key) {
    float t = (float) dt.asMilliseconds();
    std::cout << t;
    if (key == sf::Keyboard::W) {
        m_x += sin(m_theta) * sin(m_psi) * m_translationSpeed * t;
        m_y += cos(m_psi) * m_translationSpeed * t;
        m_z += cos(m_theta) * sin(m_psi) * m_translationSpeed * t;
    }
    if (key == sf::Keyboard::S) {
        m_x -= sin(m_theta) * sin(m_psi) * m_translationSpeed * t;
        m_y -= cos(m_psi) * m_translationSpeed * t;
        m_z -= cos(m_theta) * sin(m_psi) * m_translationSpeed * t;
    }
    if (key == sf::Keyboard::A) {
        m_x -= sin(m_theta - (float) std::numbers::pi / 2) * sin(m_psi) * m_translationSpeed * t;
        m_z -= cos(m_theta - (float) std::numbers::pi / 2) * sin(m_psi) * m_translationSpeed * t;
    }
    if (key == sf::Keyboard::D) {
        m_x -= sin(m_theta + (float) std::numbers::pi / 2) * sin(m_psi) * m_translationSpeed * t;
        m_z -= cos(m_theta + (float) std::numbers::pi / 2) * sin(m_psi) * m_translationSpeed * t;
    }
}