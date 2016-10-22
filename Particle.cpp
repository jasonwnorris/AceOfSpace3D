// Particle.cpp //
#include "Particle.h"

Particle::Particle(string keyname, Vector location) : Object(keyname)
{
	position = location;
	lifeInterval = sprite->texture->frameCount * sprite->texture->frameInterval;
}

void Particle::Update(float deltaTime)
{
	lifeInterval -= deltaTime;

	if(lifeInterval < 0)
	    Remove();

	Object::Update(deltaTime);
}
