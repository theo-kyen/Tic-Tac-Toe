#include <iostream>
#include "game.hpp"
using namespace std;

Game::Game()
{
    // init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Could not initialize SDL: " << SDL_GetError() << endl;
    }
    int imgFlags = IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        cout << "Could not initialize SDL_IMG: " << IMG_GetError() << endl;
    }

    // create window and renderer
    window = SDL_CreateWindow("Tic Tac Toe - SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        cout << "Could not create window:" << SDL_GetError() << endl;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        cout << "Could not create renderer:" << SDL_GetError() << endl;
    }

    board = new Board(SCREEN_SIZE);
}

void Game::Run()
{
    SDL_Event event;
    bool quit = false;
    bool is_x_turn = true;

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    while (!quit)
    {
        int x, y;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                // place a piece on the board
                bool placed = false;
                SDL_GetMouseState(&x, &y);
                if (is_x_turn)
                    placed = board->Place(renderer, Turn::X, x, y);
                else
                    placed = board->Place(renderer, Turn::O, x, y);
                if (placed)
                    is_x_turn = !is_x_turn;

                if (board->IsGameOver())
                {
                    quit = true;
                }
            }
        }
        // render board and text
        board->Render(renderer);

        // update screen
        SDL_RenderPresent(renderer);
    }

    Close();
}

void Game::Close()
{
    free(board);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    IMG_Quit();
    SDL_Quit();
}