#ifndef APPLE_H_
#define APPLE_H_

#include "LTexture.h"

//#define ENEMY_MAX_HEIGHT 300
//#define ENEMY_MIN_HEIGHT 370

#define APPLE_POSITION_RANGE 250
//#define ENEMY1_RANGE 100
//#define ENEMY2_RANGE 350
//#define ENEMY3_RANGE 500


class Apple
{
public:
    Apple();

    ~Apple();

    void LoadFromFile(std::string path, SDL_Renderer* gRenderer);

    void Move(const int& acceleration);

    void Render(SDL_Renderer* gRenderer, SDL_Rect* currentClip = nullptr);

    int GetSpeed(const int& acceleration);

    int GetPosX();

    int GetPosY();

    int GetWidth();

    int GetHeight();
private:
    int posX, posY;

    int eWidth, eHeight;

    SDL_Texture *AppleTexture;
};

#endif // !APPLE_H_
