// Star.hpp

#ifndef __STAR_HPP__
#define __STAR_HPP__

#include "Object.hpp"

class Star : public Object
{
  public:
    Star(string keyname);

    void Update(float deltaTime);
};

#endif
