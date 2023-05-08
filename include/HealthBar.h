#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "LTexture.h"

class HealthBar
{
public:
    HealthBar();

    void Render(SDL_Rect* currentClip, SDL_Renderer *gRenderer, LTexture gHealthBarTexture);

    int GetPosX();

    int GetPosY();

private:
    int posX, posY;

    int status;
};

#endif // HEALTHBAR_H
