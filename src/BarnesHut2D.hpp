#pragma once
#include <SFML/System.hpp>
#include "Particle.hpp"
#include <vector>
#include <memory>

// Opening angle (theta) parameter for Barnes-Hut
constexpr float THETA = 0.5f;

class BarnesHut2DNode {
public:
    // Bounds for this node (axis-aligned square)
    float x, y, w; // x, y: center, w: half-width

    // Child nodes
    std::unique_ptr<BarnesHut2DNode> NW, NE, SW, SE;

    // Aggregate properties
    sf::Vector2f center_of_mass = sf::Vector2f(0, 0);
    float total_mass = 0;

    // Particle held at this node (nullptr if internal node)
    Particle* particle = nullptr;

    // Constructor
    BarnesHut2DNode(float cx, float cy, float hw);

    // Insert a particle into this node/tree
    void insert(Particle* p);

    // Compute force on a given particle from this node
    sf::Vector2f computeForceOn(const Particle& p, float G) const;

    
    bool isExternal() const;
};

class BarnesHut2DTree {
public:
    std::unique_ptr<BarnesHut2DNode> root;

    BarnesHut2DTree(float width, float height);

    void insert(Particle* p);
    sf::Vector2f computeForceOn(const Particle& p, float G) const;
};
