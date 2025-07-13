#ifndef PARTICLE3D_HPP
#define PARTICLE3D_HPP

struct Particle3D {
    double x, y, z;       // Position
    double vx, vy, vz;    // Velocity
    double ax, ay, az;    // Acceleration (optional, for force update)
    double mass;

    Particle3D(double x_=0, double y_=0, double z_=0,
               double vx_=0, double vy_=0, double vz_=0,
               double mass_=1.0)
        : x(x_), y(y_), z(z_),
          vx(vx_), vy(vy_), vz(vz_),
          ax(0), ay(0), az(0),
          mass(mass_) {}

    // Utility: Reset acceleration (before force calculation)
    void reset_acceleration() {
        ax = ay = az = 0.0;
    }
};

#endif // PARTICLE3D_HPP