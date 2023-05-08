#include "HealthBar.h"

HealthBar::HealthBar()
{
    posX = 520;
    posY = 35;
}

void HealthBar::Render(SDL_Rect* currentClip, SDL_Renderer *gRenderer, LTexture gHealthBarTexture)
{
    gHealthBarTexture.Render(posX, posY, gRenderer, currentClip);
}

int HealthBar::GetPosX()
{
    return posX;
}

int HealthBar::GetPosY()
{
    return posY;
}
