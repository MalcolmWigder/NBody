#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "Particle.hpp"
#include "BarnesHut2D.hpp" 

const float G = 0.01f;
const int WIDTH = 800;
const int HEIGHT = 800;
const int NUM_PARTICLES = 8000;
const float R = 300.f;

float magnitude(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Barnes-Hut N-Body (2D)");

    std::vector<Particle> particles(NUM_PARTICLES);

    // --- Initialize particles in a disk with orbital velocities ---
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        float angle = 2 * 3.1415926f * rand() / RAND_MAX;
        float dist  = R * rand() / RAND_MAX;

        particles[i].pos = sf::Vector2f(
            WIDTH  / 2 + dist * std::cos(angle),
            HEIGHT / 2 + dist * std::sin(angle)
        );

        float v = std::sqrt(G * NUM_PARTICLES * dist / (R));
        particles[i].vel = sf::Vector2f(
            -v * std::sin(angle),
             v * std::cos(angle)
        );
    }

    sf::CircleShape shape(1.0f);

    // --- Main loop ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- Barnes-Hut Tree Step ---
        BarnesHut2DTree tree(WIDTH, HEIGHT);
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            tree.insert(&particles[i]);
        }
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            sf::Vector2f force = tree.computeForceOn(particles[i], G);
            particles[i].vel += force;
            particles[i].pos += particles[i].vel;
        }

        // --- Heatmap: compute speed min/max ---
        float min_speed = std::numeric_limits<float>::max();
        float max_speed = std::numeric_limits<float>::lowest();
        std::vector<float> speeds(NUM_PARTICLES);
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            float speed = magnitude(particles[i].vel);
            speeds[i] = speed;
            if (speed < min_speed) min_speed = speed;
            if (speed > max_speed) max_speed = speed;
        }

        // --- Draw particles with velocity color ---
        window.clear(sf::Color::Black);
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            float norm = (speeds[i] - min_speed) / (max_speed - min_speed + 1e-6f);
            sf::Color color;
            if (norm < 0.5f) {
                float t = norm * 2.0f;
                color = sf::Color(0, static_cast<sf::Uint8>(255 * t), static_cast<sf::Uint8>(255 * (1 - t)));
            } else {
                float t = (norm - 0.5f) * 2.0f;
                color = sf::Color(static_cast<sf::Uint8>(255 * t), static_cast<sf::Uint8>(255 * (1 - t)), 0);
            }
            shape.setPosition(particles[i].pos.x, particles[i].pos.y);
            shape.setFillColor(color);
            window.draw(shape);
        }
        window.display();
    }

    return 0;
}
