import pygame
import math

# Initialize pygame
pygame.init()

# Set up display
WIDTH, HEIGHT = 800, 800
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Quick Pygame Canvas")

# Set up colors
WHITE = (0, 0, 0)
YELLOW = (255, 200, 0)
GREEN = (0, 255, 0)
BLUE = (0, 100, 255)

# Gravity constant
G = 0.001

# Function to draw orbital trails
def draw_orbit(screen, trail, x, y, color):
    trail.append((x, y))
    if len(trail) > 15000:
        trail.pop(0)
    for i in range(1, len(trail)):
        pygame.draw.line(screen, color, trail[i - 1], trail[i], 1)

# Function to calculate total gravity effect on a target particle
def calculate_total_gravity(target, bodies):
    total_fx, total_fy = 0, 0
    for body in bodies:
        if body is not target:
            dx, dy = body.x - target.x, body.y - target.y
            distance = math.sqrt(dx**2 + dy**2)
            if distance == 0:
                continue
            force = G * (math.pi * target.radius**2) * (math.pi * body.radius**2) / (distance**2)
            total_fx += force * (dx / distance)
            total_fy += force * (dy / distance)
    return total_fx, total_fy

# Function to check and merge colliding particles
def check_collisions_and_merge(particles):
    to_remove = set()
    for i in range(len(particles)):
        for j in range(i + 1, len(particles)):
            body1, body2 = particles[i], particles[j]
            dx, dy = body2.x - body1.x, body2.y - body1.y
            distance = math.sqrt(dx**2 + dy**2)
            if distance <= body1.radius + body2.radius:
                larger, smaller = (body1, body2) if body1.radius > body2.radius else (body2, body1)
                larger.radius = math.sqrt((math.pi * larger.radius**2 + math.pi * smaller.radius**2) / math.pi)
                to_remove.add(smaller)
    particles[:] = [p for p in particles if p not in to_remove]

# Particle class
class Particle:
    def __init__(self, x, y, vx, vy, radius, color):
        self.x, self.y, self.vx, self.vy = x, y, vx, vy
        self.radius, self.color = radius, color
    
    def apply_force(self, fx, fy):
        mass = math.pi * self.radius**2
        self.vx += fx / mass
        self.vy += fy / mass
    
    def move(self):
        self.x += self.vx
        self.y += self.vy
    
    def draw(self, screen):
        pygame.draw.circle(screen, self.color, (int(self.x), int(self.y)), int(self.radius))

# Initialize particles
radii = [60, 20, 10]
velocities = [0, 0.25, -0.3]
offsets = [(0, 0), (-100, 200), (0, -150)]
particles = [
    Particle(WIDTH // 2 + offsets[i][0], HEIGHT // 2 + offsets[i][1], velocities[i], 0, radii[i], color)
    for i, color in enumerate([YELLOW, GREEN, BLUE])
]

# Initialize orbital trails
trails = [[] for _ in particles]

# Main loop
running = True
while running:
    screen.fill(WHITE)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    
    for i, particle in enumerate(particles):
        fx, fy = calculate_total_gravity(particle, particles)
        particle.apply_force(fx, fy)
        particle.move()
        draw_orbit(screen, trails[i], particle.x, particle.y, particle.color)
        particle.draw(screen)
    
    check_collisions_and_merge(particles)
    pygame.display.flip()

pygame.quit()
