#ifndef COLISSION_H
#define COLISSION_H

#include "Game_Base.h"
#include "Button.h"
#include "Enemy.h"
#include "Character.h"
#include "Game_Utils.h"


class Colission
{
    public:
        Colission();
        virtual ~Colission();

    bool res;

    bool CheckColission(Character character,
    SDL_Rect* char_clip,
    Enemy enemy,
    SDL_Rect* enemy_clip = nullptr);

    bool CheckEnemyColission(Character character,
    Enemy enemy1,
    Enemy enemy2,
    Enemy enemy3,
    SDL_Rect* char_clip,
    SDL_Rect* enemy_clip = nullptr);

    private:

};

#endif // COLISSION_H
