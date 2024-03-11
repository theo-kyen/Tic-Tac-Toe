#pragma once

#include "texture.hpp"
#include "board.hpp"

class Game
{
public:
    Game();
    void Run();

private:
    void Close();

    const int SCREEN_SIZE = 600;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    Board *board = NULL;
};