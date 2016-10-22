// Sprite.h //
#ifndef SPRITE_H
#define SPRITE_H

#include "vector.h"
#include "texture.h"

class Sprite
{
  public:
    Sprite(string keyname);
    
    void Update(float deltaTime);
    void Render(SDL_Surface* screen, Vector position);
    
    SDL_Rect GetFrameBounds();
    
    void SetTextureIndex(int index);
    
    Texture* texture;
    Vector origin;

  private:
    int frame;
    float frameTimeRemaining;
    
    int textureIndex;
};

#endif
