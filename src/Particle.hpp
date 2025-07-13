#pragma once
#include <SFML/System.hpp>

struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    float mass = 1.0f;
};