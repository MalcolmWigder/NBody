#include "BarnesHut2D.hpp"
#include <cmath>

// ---- BarnesHut2DNode methods ----

BarnesHut2DNode::BarnesHut2DNode(float cx, float cy, float hw)
    : x(cx), y(cy), w(hw), total_mass(0), particle(nullptr) {}

bool BarnesHut2DNode::isExternal() const {
    return !NW && !NE && !SW && !SE;
}

void BarnesHut2DNode::insert(Particle* p) {
    // If this node does not contain a particle, place p here
    if (!particle && isExternal()) {
        particle = p;
        center_of_mass = p->pos;
        total_mass = p->mass;
        return;
    }

    // Otherwise, if it's the first subdivision, subdivide and re-insert
    if (isExternal()) {
        // Subdivide
        float hw = w / 2;
        NW = std::make_unique<BarnesHut2DNode>(x - hw, y - hw, hw);
        NE = std::make_unique<BarnesHut2DNode>(x + hw, y - hw, hw);
        SW = std::make_unique<BarnesHut2DNode>(x - hw, y + hw, hw);
        SE = std::make_unique<BarnesHut2DNode>(x + hw, y + hw, hw);

        // Re-insert the already present particle
        if (particle) {
            Particle* old = particle;
            particle = nullptr;
            // Insert old particle
            if (old->pos.x <= x && old->pos.y <= y) NW->insert(old);
            else if (old->pos.x > x && old->pos.y <= y) NE->insert(old);
            else if (old->pos.x <= x && old->pos.y > y) SW->insert(old);
            else SE->insert(old);
        }
    }

    // Insert the new particle into the correct quadrant
    if (p->pos.x <= x && p->pos.y <= y) NW->insert(p);
    else if (p->pos.x > x && p->pos.y <= y) NE->insert(p);
    else if (p->pos.x <= x && p->pos.y > y) SW->insert(p);
    else SE->insert(p);

    // Update center of mass and mass
    center_of_mass = (center_of_mass * total_mass + p->pos * p->mass) / (total_mass + p->mass);
    total_mass += p->mass;
}

sf::Vector2f BarnesHut2DNode::computeForceOn(const Particle& p, float G) const {
    if (total_mass == 0 || (&p == particle)) return sf::Vector2f(0, 0);

    sf::Vector2f d = center_of_mass - p.pos;
    float dist2 = d.x * d.x + d.y * d.y + 1e-5f;
    float dist = std::sqrt(dist2);

    // Check if we can approximate or need to go deeper
    if (isExternal() || (w / dist) < THETA) {
        // Approximate as one big mass
        float invDist = 1.0f / dist;
        return G * total_mass * d * invDist * invDist;
    } else {
        // Recurse into children
        sf::Vector2f force(0, 0);
        if (NW) force += NW->computeForceOn(p, G);
        if (NE) force += NE->computeForceOn(p, G);
        if (SW) force += SW->computeForceOn(p, G);
        if (SE) force += SE->computeForceOn(p, G);
        return force;
    }
}

// ---- BarnesHut2DTree methods ----

BarnesHut2DTree::BarnesHut2DTree(float width, float height) {
    // Center at (width/2, height/2), covers the whole domain
    float hw = std::max(width, height) / 2.f;
    root = std::make_unique<BarnesHut2DNode>(width / 2.f, height / 2.f, hw);
}

void BarnesHut2DTree::insert(Particle* p) {
    root->insert(p);
}

sf::Vector2f BarnesHut2DTree::computeForceOn(const Particle& p, float G) const {
    return root->computeForceOn(p, G);
}
