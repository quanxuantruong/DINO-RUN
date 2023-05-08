#include "Game_Base.h"
#include "Game_Utils.h"
#include "LTexture.h"
#include "Button.h"
#include "Character.h"
#include "Enemy.h"
#include "Apple.h"
#include "HealthBar.h"


const std::string LAYER[BACKGROUND_LAYER] =
{
    "imgs/background/layer01.png",
    "imgs/background/layer02.png",
    "imgs/background/layer03.png",
    "imgs/background/layer04.png",
    "imgs/background/layer05.png",
};

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Color textColor = { 255, 255, 255 };
SDL_Color textColor2 = { 170, 0, 0 };
TTF_Font* gFont = nullptr;
TTF_Font* gFont2 = nullptr;
Mix_Music* gMusic = nullptr;
Mix_Music* gMenuMusic = nullptr;
Mix_Chunk* gClick = nullptr;
Mix_Chunk* gJump = nullptr;
Mix_Chunk* gLose = nullptr;

SDL_Rect gPlayButton[BUTTON_TOTAL];
SDL_Rect gHelpButton[BUTTON_TOTAL];
SDL_Rect gExitButton[BUTTON_TOTAL];
SDL_Rect gBackButton[BUTTON_TOTAL];
SDL_Rect gPauseButton[BUTTON_TOTAL];
SDL_Rect gContinueButton[BUTTON_TOTAL];
SDL_Rect gPlayAgainButton[BUTTON_TOTAL];
SDL_Rect gCharacterClips[RUNNING_FRAMES];
SDL_Rect gHealthBarClips[HEALTHBAR_FRAMES];
SDL_Rect gEnemyClips[FLYING_FRAMES];
SDL_Rect gAppleClips[APPLE_FRAMES];

LTexture gMenuTexture;
LTexture gInstructionTexture;
LTexture gBackgroundTexture[BACKGROUND_LAYER];
LTexture gCharacterTexture;
LTexture gHealthBarTexture;
LTexture gGroundTexture;
LTexture gPlayButtonTexture;
LTexture gHelpButtonTexture;
LTexture gExitButtonTexture;
LTexture gBackButtonTexture;
LTexture gPauseButtonTexture;
LTexture gContinueButtonTexture;
LTexture gLoseTexture;
LTexture gText1Texture;
LTexture gScoreTexture;
LTexture gText2Texture;
LTexture gHighScoreTexture;
LTexture gText3Texture;
LTexture gHPTexture;

Button PlayButton(PLAY_BUTON_POSX, PLAY_BUTTON_POSY);
Button HelpButton(HELP_BUTTON_POSX, HELP_BUTTON_POSY);
Button ExitButton(EXIT_BUTTON_POSX, EXIT_BUTTON_POSY);
Button BackButton(BACK_BUTTON_POSX, BACK_BUTTON_POSY);
Button PauseButton(PAUSE_BUTTON_POSX, PAUSE_BUTTON_POSY);
Button ContinueButton(CONTINUE_BUTTON_POSX, CONTINUE_BUTTON_POSY);

Character character;

HealthBar healthbar;

int main(int argc, char* argv[])
{
    if (!Init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        if (!LoadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            bool Quit_Menu = false;
            bool Play_Again = false;

            Mix_PlayMusic(gMenuMusic, IS_REPEATITIVE);
            while (!Quit_Menu)
            {
                SDL_Event e_mouse;
                while (SDL_PollEvent(&e_mouse) != 0)
                {
                    if (e_mouse.type == SDL_QUIT)
                    {
                        Quit_Menu = true;
                    }

                    bool Quit_Game = false;
                    HandlePlayButton(&e_mouse, PlayButton, Quit_Menu, Play_Again, gClick);

                    HandleHelpButton(&e_mouse, gBackButton,
                                     HelpButton, BackButton,
                                     gInstructionTexture, gBackButtonTexture,
                                     gRenderer, Quit_Game, gClick);

                    HandleExitButton(&e_mouse, ExitButton, Quit_Menu, gClick);

                    if (Quit_Game == true)
                    {
                        return 0;
                    }
                }

                gMenuTexture.Render(0, 0, gRenderer);

                SDL_Rect* currentClip_Play = &gPlayButton[PlayButton.currentSprite];
                PlayButton.Render(currentClip_Play, gRenderer, gPlayButtonTexture);

                SDL_Rect* currentClip_Help = &gHelpButton[HelpButton.currentSprite];
                HelpButton.Render(currentClip_Help, gRenderer, gHelpButtonTexture);

                SDL_Rect* currentClip_Exit = &gExitButton[ExitButton.currentSprite];
                ExitButton.Render(currentClip_Exit, gRenderer, gExitButtonTexture);

                SDL_RenderPresent(gRenderer);
            }

            while (Play_Again)
            {
                srand(time(NULL));
                int time = 0;
                int score = 0;
                int acceleration = 0;
                int frame_Character = 0;
                int frame_Enemy = 0;
                int frame_Apple = 0;
                int frame_HealthBar = 0;
                std::string highscore = GetHighScoreFromFile("high_score.txt");

                SDL_Event e;
                Enemy enemy1(ON_GROUND_ENEMY);
                Enemy enemy2(ON_GROUND_ENEMY);
                Enemy enemy3(IN_AIR_ENEMY);
                Apple apple;

                Mix_PlayMusic(gMusic, IS_REPEATITIVE);
                GenerateEnemy(enemy1, enemy2, enemy3, gEnemyClips, gRenderer);
                GenerateApple(apple, gAppleClips,gRenderer);

                int OffsetSpeed_Ground = BASE_OFFSET_SPEED;
                std::vector <double> OffsetSpeed_Bkgr(BACKGROUND_LAYER, BASE_OFFSET_SPEED);

                bool Quit = false;
                bool Game_State = true;

                int HP = 100;
                while (!Quit)
                {
                    if (Game_State)
                    {

                        UpdateGameTimeAndScore(time, acceleration, score);

                        while (SDL_PollEvent(&e) != 0)
                        {
                            if (e.type == SDL_QUIT)
                            {
                                Quit = true;
                                Play_Again = false;
                            }

                            HandlePauseButton(e, gRenderer, gContinueButton,
                                              PauseButton, ContinueButton,
                                              gContinueButtonTexture, Game_State, gClick);

                            character.HandleEvent(e, gJump);
                        }

                        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                        SDL_RenderClear(gRenderer);

                        RenderScrollingBackground(OffsetSpeed_Bkgr, gBackgroundTexture, gRenderer);
                        RenderScrollingGround(OffsetSpeed_Ground, acceleration, gGroundTexture, gRenderer);

                        character.Move();
                        SDL_Rect* currentClip_Character = nullptr;
                        if (character.OnGround())
                        {
                            currentClip_Character = &gCharacterClips[frame_Character / SLOW_FRAME_CHAR];
                            character.Render(currentClip_Character, gRenderer, gCharacterTexture);
                        }
                        else
                        {
                            currentClip_Character = &gCharacterClips[0];
                            character.Render(currentClip_Character, gRenderer, gCharacterTexture);
                        }

                        SDL_Rect* currentClip_HealthBar = nullptr;
                        currentClip_HealthBar = &gHealthBarClips[frame_HealthBar];
                        healthbar.Render(currentClip_HealthBar, gRenderer, gHealthBarTexture);

                        enemy1.Move(acceleration);
                        enemy1.Render(gRenderer);

                        enemy2.Move(acceleration);
                        enemy2.Render(gRenderer);

                        SDL_Rect* currentClip_Enemy = &gEnemyClips[frame_Enemy / SLOW_FRAME_ENEMY];
                        enemy3.Move(acceleration);
                        enemy3.Render(gRenderer, currentClip_Enemy);

                        SDL_Rect* currentClip_Apple = &gAppleClips[frame_Apple];
                        apple.Move(acceleration);
                        apple.Render(gRenderer, currentClip_Apple);

                        SDL_Rect* currentClip_Pause = &gPauseButton[PauseButton.currentSprite];
                        PauseButton.Render(currentClip_Pause, gRenderer, gPauseButtonTexture);

                        DrawPlayerScore(gText1Texture, gScoreTexture, textColor, gRenderer, gFont, score);
                        DrawPlayerHighScore(gText2Texture, gHighScoreTexture, textColor, gRenderer, gFont, highscore);
                        //DrawHPScore(gText3Texture, gHPTexture, textColor2, gRenderer, gFont2, HP);


                        if (CheckEnemyColission(character,
                                                enemy1, enemy2, enemy3,
                                                currentClip_Character, currentClip_Enemy))
                        {
                            HP = HP - 1;
                            std::cout << HP << std::endl;
                            Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);
                        }

                        else if (CheckAppleColission(character,
                                                     apple,
                                                     currentClip_Character, currentClip_Apple))
                        {
                            if ((HP + 1) <= 100) HP = HP + 1;
                            std::cout << HP << std::endl;
                            Mix_PlayChannel(MIX_CHANNEL, gLose, NOT_REPEATITIVE);

                        }

                        if (HP <= -1)
                        {
                            Mix_PauseMusic();
                            UpdateHighScore("high_score.txt", score, highscore);
                            Quit = true;
                        }

                        SDL_RenderPresent(gRenderer);

                        ControlHealthBarFrame(frame_HealthBar, HP);
                        ControlCharFrame(frame_Character);
                        ControlEnemyFrame(frame_Enemy);
                        ControlAppleFrame(frame_Apple, CheckAppleColission(character,
                                                     apple,
                                                     currentClip_Character, currentClip_Apple), apple.GetPosX() );

                    }
                }

                DrawEndGameSelection(gLoseTexture, &e, gRenderer, Play_Again);
                if (!Play_Again)
                {
                    enemy1.~Enemy();
                    enemy2.~Enemy();
                    enemy3.~Enemy();
                    apple.~Apple();
                }
            }
        }
    }
    Close();

    return 0;
}


bool Init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO) < 0)
    {
        LogError("Can not initialize SDL.", SDL_ERROR);
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "Warning: Linear texture filtering not enabled!";
        }

        gWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            LogError("Can not create window", SDL_ERROR);
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                LogError("Can not create renderer", SDL_ERROR);
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    LogError("Can not initialize SDL_image", IMG_ERROR);
                    success = false;
                }

                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }

                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool LoadMedia()
{
    bool success = true;

    gMusic = Mix_LoadMUS("sound/bkgr_audio.wav");
    if (gMusic == nullptr)
    {
        LogError("Failed to load background music", MIX_ERROR);
        success = false;
    }

    gMenuMusic = Mix_LoadMUS("sound/menu_audio.wav");
    if (gMenuMusic == nullptr)
    {
        LogError("Failed to load menu music", MIX_ERROR);
        success = false;
    }

    gClick = Mix_LoadWAV("sound/mouse_click.wav");
    if (gClick == nullptr)
    {
        LogError("Failed to load mouse click sound", MIX_ERROR);
        success = false;
    }

    gJump = Mix_LoadWAV("sound/jump_sound.wav");
    if (gJump == nullptr)
    {
        LogError("Failed to load jumping sound", MIX_ERROR);
        success = false;
    }

    gLose = Mix_LoadWAV("sound/lose_sound.wav");
    if (gLose == nullptr)
    {
        LogError("Failed to load lose sound", MIX_ERROR);
        success = false;
    }

    else
    {
        gFont = TTF_OpenFont("font/pixel_font.ttf", 28);
        gFont2 = TTF_OpenFont("font/pixel_font.ttf", 40);
        if (gFont == NULL)
        {
            LogError("Failed to load font", MIX_ERROR);
            success = false;
        }
        else
        {
            if (!gText1Texture.LoadFromRenderedText("Your score: ", gFont, textColor, gRenderer))
            {
                std::cout << "Failed to render text1 texture" << std::endl;
                success = false;
            }

            if (!gText2Texture.LoadFromRenderedText("High score: ", gFont, textColor, gRenderer))
            {
                std::cout << "Failed to render text2 texture" << std::endl;
                success = false;
            }
            if (!gText3Texture.LoadFromRenderedText("HP: ", gFont2, textColor2, gRenderer))
            {
                std::cout << "Failed to render text3 texture" << std::endl;
                success = false;
            }

            if (!gMenuTexture.LoadFromFile("imgs/background/menu.png", gRenderer))
            {
                std::cout << "Failed to load menu image" << std::endl;
                success = false;
            }

            if (!gInstructionTexture.LoadFromFile("imgs/background/instruction.png", gRenderer))
            {
                std::cout << "Failed to load instruction image" << std::endl;
                success = false;
            }

            if (!gPlayButtonTexture.LoadFromFile("imgs/button/big_button/play_button.png", gRenderer))
            {
                std::cout << "Failed to load play_button image" << std::endl;
                success = false;
            }
            else
            {
                for (int i = 0; i < BUTTON_TOTAL; ++i)
                {
                    gPlayButton[i].x = 150 * i;
                    gPlayButton[i].y = 0;
                    gPlayButton[i].w = 150;
                    gPlayButton[i].h = 98;
                }
            }

            if (!gHelpButtonTexture.LoadFromFile("imgs/button/big_button/help_button.png", gRenderer))
            {
                std::cout << "Failed to load help_button image" << std::endl;
                success = false;
            }
            else
            {
                for (int i = 0; i < BUTTON_TOTAL; ++i)
                {
                    gHelpButton[i].x = 150 * i;
                    gHelpButton[i].y = 0;
                    gHelpButton[i].w = 150;
                    gHelpButton[i].h = 98;
                }
            }

            if (!gBackButtonTexture.LoadFromFile("imgs/button/big_button/back_button.png", gRenderer))
            {
                std::cout << "Failed to load back_button image" << std::endl;
                success = false;
            }
            else
            {
                for (int i = 0; i < BUTTON_TOTAL; ++i)
                {
                    gBackButton[i].x = 100 * i;
                    gBackButton[i].y = 0;
                    gBackButton[i].w = 100;
                    gBackButton[i].h = 78;
                }
            }

            if (!gExitButtonTexture.LoadFromFile("imgs/button/big_button/exit_button.png", gRenderer))
            {
                std::cout << "Failed to load exit_button image" << std::endl;
                success = false;
            }
            else
            {
                for (int i = 0; i < BUTTON_TOTAL; ++i)
                {
                    gExitButton[i].x = 150 * i;
                    gExitButton[i].y = 0;
                    gExitButton[i].w = 150;
                    gExitButton[i].h = 98;
                }
            }

            if (!gPauseButtonTexture.LoadFromFile("imgs/button/big_button/pause_button.png", gRenderer))
            {
                std::cout << "Failed to load pause_button image " << std::endl;
                success = false;
            }
            else
            {
                for (int i = 0; i < BUTTON_TOTAL; ++i)
                {
                    gPauseButton[i].x = 22 * i;
                    gPauseButton[i].y = 0;
                    gPauseButton[i].w = 22;
                    gPauseButton[i].h = 34;
                }
            }

            if (!gContinueButtonTexture.LoadFromFile("imgs/button/big_button/continue_button.png", gRenderer))
            {
                std::cout << "Failed to load continue_button image " << std::endl;
                success = false;
            }
            else
            {
                for (int i = 0; i < BUTTON_TOTAL; ++i)
                {
                    gContinueButton[i].x = 22 * i;
                    gContinueButton[i].y = 0;
                    gContinueButton[i].w = 22;
                    gContinueButton[i].h = 34;
                }
            }

            for (int i = 0; i < BACKGROUND_LAYER; ++i)
            {
                if (!gBackgroundTexture[i].LoadFromFile(LAYER[i].c_str(), gRenderer))
                {
                    std::cout << "Failed to load background image" << std::endl;
                    success = false;
                }
            }

            if (!gGroundTexture.LoadFromFile("imgs/background/ground.png", gRenderer))
            {
                std::cout << "Failed to load ground image" << std::endl;
                success = false;
            }

            if (!gCharacterTexture.LoadFromFile("imgs/character/char.png", gRenderer))
            {
                std::cout << "Failed to load character_run image." << std::endl;
                success = false;
            }
            else
            {
                gCharacterClips[0].x = 75 * 0;
                gCharacterClips[0].y = 0;
                gCharacterClips[0].w = 75;
                gCharacterClips[0].h = 85;

                gCharacterClips[1].x = 75 * 1;
                gCharacterClips[1].y = 0;
                gCharacterClips[1].w = 75;
                gCharacterClips[1].h = 85;

                gCharacterClips[2].x = 75 * 2;
                gCharacterClips[2].y = 0;
                gCharacterClips[2].w = 75;
                gCharacterClips[2].h = 85;

                gCharacterClips[3].x = 75 * 3;
                gCharacterClips[3].y = 0;
                gCharacterClips[3].w = 75;
                gCharacterClips[3].h = 85;

                gCharacterClips[4].x = 75 * 4;
                gCharacterClips[4].y = 0;
                gCharacterClips[4].w = 75;
                gCharacterClips[4].h = 85;

                gCharacterClips[5].x = 75 * 5;
                gCharacterClips[5].y = 0;
                gCharacterClips[5].w = 75;
                gCharacterClips[5].h = 85;
            }

            if (!gHealthBarTexture.LoadFromFile("imgs/healthbar/healthbar.png", gRenderer))
            {
                std::cout << "Failed to load healthbar image." << std::endl;
                success = false;
            }
            else
            {
                gHealthBarClips[0].x = 0;
                gHealthBarClips[0].y = 50 * 0;
                gHealthBarClips[0].w = 245;
                gHealthBarClips[0].h = 50;

                gHealthBarClips[1].x = 0;
                gHealthBarClips[1].y = 50 * 1;
                gHealthBarClips[1].w = 245;
                gHealthBarClips[1].h = 50;

                gHealthBarClips[2].x = 0;
                gHealthBarClips[2].y = 50 * 2;
                gHealthBarClips[2].w = 245;
                gHealthBarClips[2].h = 50;

                gHealthBarClips[3].x = 0;
                gHealthBarClips[3].y = 50 * 3;
                gHealthBarClips[3].w = 245;
                gHealthBarClips[3].h = 50;

                gHealthBarClips[4].x = 0;
                gHealthBarClips[4].y = 50 * 4;
                gHealthBarClips[4].w = 245;
                gHealthBarClips[4].h = 50;

                gHealthBarClips[5].x = 0;
                gHealthBarClips[5].y = 50 * 5;
                gHealthBarClips[5].w = 245;
                gHealthBarClips[5].h = 50;

                gHealthBarClips[6].x = 0;
                gHealthBarClips[6].y = 50 * 6;
                gHealthBarClips[6].w = 245;
                gHealthBarClips[6].h = 50;

                gHealthBarClips[7].x = 0;
                gHealthBarClips[7].y = 50 * 7;
                gHealthBarClips[7].w = 245;
                gHealthBarClips[7].h = 50;

                gHealthBarClips[8].x = 0;
                gHealthBarClips[8].y = 50 * 8;
                gHealthBarClips[8].w = 245;
                gHealthBarClips[8].h = 50;

                gHealthBarClips[9].x = 0;
                gHealthBarClips[9].y = 50 * 9;
                gHealthBarClips[9].w = 245;
                gHealthBarClips[9].h = 50;

                gHealthBarClips[10].x = 0;
                gHealthBarClips[10].y = 50 * 10;
                gHealthBarClips[10].w = 245;
                gHealthBarClips[10].h = 50;

                gHealthBarClips[11].x = 0;
                gHealthBarClips[11].y = 50 * 11;
                gHealthBarClips[11].w = 245;
                gHealthBarClips[11].h = 50;

                gHealthBarClips[12].x = 0;
                gHealthBarClips[12].y = 50 * 12;
                gHealthBarClips[12].w = 245;
                gHealthBarClips[12].h = 50;

                gHealthBarClips[13].x = 0;
                gHealthBarClips[13].y = 50 * 13;
                gHealthBarClips[13].w = 245;
                gHealthBarClips[13].h = 50;

                gHealthBarClips[14].x = 0;
                gHealthBarClips[14].y = 50 * 14;
                gHealthBarClips[14].w = 245;
                gHealthBarClips[14].h = 50;

                gHealthBarClips[15].x = 0;
                gHealthBarClips[15].y = 50 * 15;
                gHealthBarClips[15].w = 245;
                gHealthBarClips[15].h = 50;

                gHealthBarClips[16].x = 0;
                gHealthBarClips[16].y = 50 * 16;
                gHealthBarClips[16].w = 245;
                gHealthBarClips[16].h = 50;

                gHealthBarClips[17].x = 0;
                gHealthBarClips[17].y = 50 * 17;
                gHealthBarClips[17].w = 245;
                gHealthBarClips[17].h = 50;

                gHealthBarClips[18].x = 0;
                gHealthBarClips[18].y = 50 * 18;
                gHealthBarClips[18].w = 245;
                gHealthBarClips[18].h = 50;

                gHealthBarClips[19].x = 0;
                gHealthBarClips[19].y = 50 * 19;
                gHealthBarClips[19].w = 245;
                gHealthBarClips[19].h = 50;

                gHealthBarClips[20].x = 0;
                gHealthBarClips[20].y = 50 * 20;
                gHealthBarClips[20].w = 245;
                gHealthBarClips[20].h = 50;

                gHealthBarClips[21].x = 0;
                gHealthBarClips[21].y = 50 * 21;
                gHealthBarClips[21].w = 245;
                gHealthBarClips[21].h = 50;

                gHealthBarClips[22].x = 0;
                gHealthBarClips[22].y = 50 * 22;
                gHealthBarClips[22].w = 245;
                gHealthBarClips[22].h = 50;

                gHealthBarClips[23].x = 0;
                gHealthBarClips[23].y = 50 * 23;
                gHealthBarClips[23].w = 245;
                gHealthBarClips[23].h = 50;

                gHealthBarClips[24].x = 0;
                gHealthBarClips[24].y = 50 * 24;
                gHealthBarClips[24].w = 245;
                gHealthBarClips[24].h = 50;

                gHealthBarClips[25].x = 0;
                gHealthBarClips[25].y = 50 * 25;
                gHealthBarClips[25].w = 245;
                gHealthBarClips[25].h = 50;

                gHealthBarClips[26].x = 0;
                gHealthBarClips[26].y = 50 * 26;
                gHealthBarClips[26].w = 245;
                gHealthBarClips[26].h = 50;

            }


            if (!gLoseTexture.LoadFromFile("imgs/background/lose.png", gRenderer))
            {
                std::cout << "Failed to load lose image." << std::endl;
                success = false;
            }
        }
    }
    return success;
}

void Close()
{
    gMenuTexture.Free();
    gInstructionTexture.Free();
    gCharacterTexture.Free();
    gGroundTexture.Free();
    gPlayButtonTexture.Free();
    gHelpButtonTexture.Free();
    gExitButtonTexture.Free();
    gBackButtonTexture.Free();
    gPauseButtonTexture.Free();
    gContinueButtonTexture.Free();
    gLoseTexture.Free();
    gText1Texture.Free();
    gScoreTexture.Free();
    gText2Texture.Free();
    gHighScoreTexture.Free();
    gText3Texture.Free();
    gHPTexture.Free();

    for (int i = 0; i < BACKGROUND_LAYER; ++i)
    {
        gBackgroundTexture[i].Free();
    }

    Mix_FreeMusic(gMusic);
    Mix_FreeMusic(gMenuMusic);
    Mix_FreeChunk(gClick);
    Mix_FreeChunk(gLose);
    Mix_FreeChunk(gJump);
    gMusic = nullptr;
    gMenuMusic = nullptr;
    gClick = nullptr;
    gLose = nullptr;
    gJump = nullptr;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}
