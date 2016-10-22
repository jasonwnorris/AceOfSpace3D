// Fireball.h //
#ifndef FIREBALL_H
#define FIREBALL_H

#include "Projectile.h"

class Fireball : public Projectile
{
  public:
    Fireball(string keyname);
    
    void Update(float deltaTime);
    
  private:
    float lifeTimeRemaining;
};

#endif
