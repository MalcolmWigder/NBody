#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "Particle.hpp"

const float G = 10.f;
const int WIDTH = 800;
const int HEIGHT = 800;
const int NUM_PARTICLES = 3000;
const float R = 200.f;

float magnitude(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "N-Body Simulation");
    std::vector<Particle> particles(NUM_PARTICLES);

    // Initialize particles in a cluster
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        float angle = 2 * 3.1415926f * rand() / RAND_MAX;
        float dist = R * rand() / RAND_MAX;
        particles[i].pos = sf::Vector2f(
            WIDTH / 2 + dist * std::cos(angle),
            HEIGHT / 2 + dist * std::sin(angle)
        );
        // Small initial tangential velocity
        float v = std::sqrt(G * NUM_PARTICLES * dist / (R * R));
        particles[i].vel = sf::Vector2f(-v * std::sin(angle), v * std::cos(angle));
    }

    sf::CircleShape shape(1.0f);

    while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    // --- Physics: Center of Mass gravity ---
    sf::Vector2f com(0.f, 0.f);
    float total_mass = 0.f;
    for (const auto& p : particles) {
        com += p.mass * p.pos;
        total_mass += p.mass;
    }
    if (total_mass > 0) com /= total_mass;

    for (auto& p : particles) {
        sf::Vector2f diff = com - p.pos;
        float dist2 = diff.x * diff.x + diff.y * diff.y + 1e-5f;
        float invDist = 1.0f / std::sqrt(dist2);
        sf::Vector2f force = G * total_mass * diff * invDist * invDist * invDist;
        p.vel += force;
        p.pos += p.vel;
    }

    window.clear(sf::Color::Black);
    for (const auto& p : particles) {
        shape.setPosition(p.pos.x, p.pos.y);
        shape.setFillColor(sf::Color::White);
        window.draw(shape);
    }
    window.display();
}

    return 0;
}
