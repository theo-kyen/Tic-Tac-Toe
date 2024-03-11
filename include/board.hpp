#pragma once

#include <vector>
#include "texture.hpp"

// whether the current player is X or O
enum class Turn : char
{
    X = 'X',
    O = 'O',
    None
};

// represents a move on the board
struct Move
{
    Turn turn;
    int row, col; // the coordinates of the move on the grid
};

// displays the board to the screen
class Board
{
public:
    Board(int size)
    {
        m_screen_size = size;
    }
    ~Board()
    {
        m_screen_size = 0;
    }

    // given the turn and mouse coordinates, places a move on the board
    // returns true if a move was successful
    bool Place(SDL_Renderer *renderer, Turn turn, int x, int y);
    // renders the board to the screen
    void Render(SDL_Renderer *renderer);
    // determines whether or not the game is over
    bool IsGameOver();

private:
    int m_screen_size;
    std::vector<Move> m_moves;

    // get the correct grid position given the position of the mouse
    std::vector<int> GetGridPosition(int x, int y, int cell_size, int low, int high);
    // renders the given move to the screen
    void RenderMove(SDL_Renderer *renderer, Move move, SDL_Rect rect);
    // returns the turn of the move at the given location
    Turn GetMove(int row, int col);
    // returns true if every column has the same move
    bool CheckColumns();
    // returns true if every row has the same move
    bool CheckRows();
    // returns true if every diagonal has the same move
    bool CheckDiagonals();
};