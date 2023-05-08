#include "Apple.h"

Apple::Apple()
{
    posX = 0;
    posY = 0;

    eWidth = 0;
    eHeight = 0;

        posX = rand() % (SCREEN_WIDTH + APPLE_POSITION_RANGE) + SCREEN_WIDTH;
        posY = 520;
        //posY = rand() % (ENEMY_MAX_HEIGHT - ENEMY_MIN_HEIGHT + 1) + ENEMY_MIN_HEIGHT;

    AppleTexture = nullptr;
}

Apple::~Apple()
{
    posX = 0;
    posY = 0;

    eWidth = 0;
    eHeight = 0;

    if (AppleTexture != nullptr)
    {
        AppleTexture = nullptr;
    }
}

void Apple::LoadFromFile(std::string path, SDL_Renderer* gRenderer)
{
    SDL_Texture* tmpTexture = nullptr;

    SDL_Surface* tmpSurface = IMG_Load(path.c_str());
    if (tmpSurface == nullptr)
    {
        LogError("Can not load image.", IMG_ERROR);
    }
    else
    {
        SDL_SetColorKey(tmpSurface, SDL_TRUE, SDL_MapRGB(tmpSurface->format, 0, 255, 255));

        tmpTexture = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);
        if (tmpTexture == nullptr)
        {
            LogError("Can not create texture from surface.", SDL_ERROR);
        }
        else
        {
            eWidth = tmpSurface->w;
            eHeight = tmpSurface->h;
        }

        SDL_FreeSurface(tmpSurface);
    }

    AppleTexture = tmpTexture;
}

void Apple::Move(const int &acceleration)
{
    posX += -(ENEMY_SPEED + acceleration);
    if (posX + MAX_ENEMY_WIDTH < 0)
    {
        posX = rand() % (SCREEN_WIDTH + APPLE_POSITION_RANGE) + SCREEN_WIDTH;
        posY = 520;
    }
}

void Apple::Render(SDL_Renderer* gRenderer, SDL_Rect* currentClip)
{
    SDL_Rect renderSpace = { posX, posY, eWidth, eHeight };
    if (currentClip != nullptr)
    {
        renderSpace.w = currentClip->w;
        renderSpace.h = currentClip->h;
    }
    SDL_RenderCopy(gRenderer, AppleTexture, currentClip, &renderSpace);
}

int Apple::GetSpeed(const int &acceleration)
{
    return ENEMY_SPEED + acceleration;
}

int Apple::GetPosX()
{
    return posX;
}

int Apple::GetPosY()
{
    return posY;
}

int Apple::GetWidth()
{
    return eWidth;
}

int Apple::GetHeight()
{
    return eHeight;
}
