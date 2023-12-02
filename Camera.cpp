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
    m_time(0) {}


void Camera::rotation(int newMouseX, int newMouseY) {
    m_theta -= static_cast<float>(newMouseX  - m_mouseX) * m_rotationSpeed;
    m_psi += static_cast<float>(newMouseY - m_mouseY) * m_rotationSpeed;
    if (m_psi <= 0.1) m_psi = 0.1f;
    else if (m_psi >= 0.95f * (float) std::numbers::pi) m_psi = 0.95f * (float) std::numbers::pi;
    m_mouseX = newMouseX;
    m_mouseY = newMouseY;
}

void Camera::translation(sf::Time dt, sf::Keyboard::Key key) {
    float t = (float) dt.asMilliseconds();
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