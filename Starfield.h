// Starfield.h //
#ifndef STARFIELD_H
#define STARFIELD_H

#include "Star.h"

class Starfield
{
	public:
		Starfield();
		
		void Update(float deltaTime);
		
	private:
		float lastSpawn;
};

#endif
