// Particle.hpp

#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__

#include "Object.hpp"

class Particle : public Object
{
  public:
    Particle(string keyname, Vector location);

    void Update(float deltaTime);

  private:
    float lifeInterval;
};

#endif
