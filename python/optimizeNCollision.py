import pygame
import numpy as np

# Initialize pygame
pygame.init()

# Set up display
WIDTH, HEIGHT = 800, 800
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Two Spinning Galaxies with NumPy Optimization")

# Set up colors
WHITE = (255, 255, 255)
BLACK = (200, 50, 0)

# Constants
G = 0.04  # The gravitational constant
PRESSURE_STRENGTH = 0.005  # Repulsion force strength
PRESSURE_DISTANCE = 20  # Distance threshold for repulsion
NUM_PARTICLES_PER_GALAXY = 1000  # Particles per galaxy
GALAXY_SEPARATION = 400  # Distance between galaxy centers
R = 200  # Cluster radius

# Total particles
NUM_PARTICLES = NUM_PARTICLES_PER_GALAXY * 2

# Initialize particle positions and velocities
angles = np.random.uniform(0, 2 * np.pi, NUM_PARTICLES)
distances = np.random.uniform(0, R, NUM_PARTICLES)

# Create two galaxy centers
galaxy_centers = np.array([
    [WIDTH // 2 - GALAXY_SEPARATION // 2, HEIGHT // 2+40],  # Left galaxy
    [WIDTH // 2 + GALAXY_SEPARATION // 2, HEIGHT // 2]   # Right galaxy
])

# Assign each particle to a galaxy
galaxy_indices = np.repeat([0, 1], NUM_PARTICLES_PER_GALAXY)

# Positions
positions = np.zeros((NUM_PARTICLES, 2))
positions[:, 0] = galaxy_centers[galaxy_indices, 0] + distances * np.cos(angles)
positions[:, 1] = galaxy_centers[galaxy_indices, 1] + distances * np.sin(angles)

# Velocities (Orbital motion around galaxy center)
speeds = np.sqrt(G * NUM_PARTICLES_PER_GALAXY) * (distances / R)
velocities = np.zeros((NUM_PARTICLES, 2))
velocities[:, 0] = -speeds * np.sin(angles)
velocities[:, 1] = speeds * np.cos(angles)

# Radii
radii = np.ones(NUM_PARTICLES)

# Function to calculate forces using NumPy
def calculate_forces(positions):
    dx = positions[:, np.newaxis, 0] - positions[np.newaxis, :, 0]
    dy = positions[:, np.newaxis, 1] - positions[np.newaxis, :, 1]
    distance_sq = dx**2 + dy**2 + 1e-5  # Avoid division by zero
    inv_distance = 1 / np.sqrt(distance_sq)

    # Apply gravity (ignoring self-force)
    gravity_forces = G * inv_distance
    gravity_forces[np.diag_indices(NUM_PARTICLES)] = 0  # Zero out self-interactions
    fx = -np.sum(gravity_forces * dx * inv_distance, axis=1)
    fy = -np.sum(gravity_forces * dy * inv_distance, axis=1)

    # Apply repulsion force within threshold
    mask = distance_sq < PRESSURE_DISTANCE
    pressure_forces = PRESSURE_STRENGTH * inv_distance * mask
    fx += np.sum(pressure_forces * dx * inv_distance, axis=1)
    fy += np.sum(pressure_forces * dy * inv_distance, axis=1)

    return fx, fy

# Main loop
running = True
while running:
    screen.fill(WHITE)
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Compute forces
    fx, fy = calculate_forces(positions)

    # Apply forces and update velocities
    velocities[:, 0] += fx
    velocities[:, 1] += fy

    # Move particles
    positions += velocities

    # Draw particles
    for i in range(NUM_PARTICLES):
        pygame.draw.circle(screen, BLACK, (int(positions[i, 0]), int(positions[i, 1])), int(radii[i]))

    pygame.display.flip()

pygame.quit()
