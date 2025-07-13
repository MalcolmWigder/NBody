#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>      // <--- Add this!
#include "Particle.hpp"

const float G = 0.01f;
const int WIDTH = 800;
const int HEIGHT = 800;
const int NUM_PARTICLES = 1000;
const float R = 200.f;

float magnitude(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "N-Body Simulation");

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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- Compute accelerations ---
        std::vector<sf::Vector2f> acc(NUM_PARTICLES, sf::Vector2f(0, 0));
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            for (int j = 0; j < NUM_PARTICLES; ++j) {
                if (i == j) continue;
                sf::Vector2f diff = particles[j].pos - particles[i].pos;
                float dist2 = diff.x * diff.x + diff.y * diff.y + 1e-5f;
                float invDist = 1.0f / std::sqrt(dist2);
                acc[i] += G * particles[j].mass * diff * invDist  * invDist;
            }
        }

        // --- Update velocities and positions ---
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            particles[i].vel += acc[i];
            particles[i].pos += particles[i].vel;
        }

        // --- Draw particles with velocity heatmap ---
        window.clear(sf::Color::Black);

        // 1. Calculate min/max speeds
        float min_speed = std::numeric_limits<float>::max();
        float max_speed = std::numeric_limits<float>::lowest();
        std::vector<float> speeds(NUM_PARTICLES);
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            float speed = magnitude(particles[i].vel);
            speeds[i] = speed;
            if (speed < min_speed) min_speed = speed;
            if (speed > max_speed) max_speed = speed;
        }

        // 2. Draw with heatmap color
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            float norm = (speeds[i] - min_speed) / (max_speed - min_speed + 1e-6f);
            // Blue to Green to Red heatmap
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
