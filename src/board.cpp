#include <iostream>
#include <math.h>
#include "board.hpp"
using namespace std;

bool Board::Place(SDL_Renderer *renderer, Turn turn, int x, int y)
{
    int cell_size = m_screen_size / 6;
    int low = int(cell_size * 1.5);  // the highest and leftmost position of the grid
    int high = int(cell_size * 4.5); // the lowest and rightmost position of the grid

    // ensure the click happens on the board
    if (x < low || x > high || y < low || y > high)
        return false;

    // calculate grid position and ensure a move hasn't already been placed at the position
    vector<int> coords = GetGridPosition(x, y, cell_size, low, high);
    int row = coords[0];
    int col = coords[1];
    for (Move move : m_moves)
    {
        if (move.row == row && move.col == col)
        {
            return false;
        }
    }

    // add move to the board
    Move move = {turn, row, col};
    m_moves.push_back(move);

    return true;
}

void Board::Render(SDL_Renderer *renderer)
{
    int length = m_screen_size / 2;
    int cell_size = m_screen_size / 6;
    int top_left = int(cell_size * 1.5); // coordinate of the top left of each rectangle
    int left = int(cell_size * 2.5);     // coordinate for the left-side rectangles
    int right = int(cell_size * 3.5);    // coordinate for the right-side rectangles

    // defining the rectangles for the grid
    SDL_Rect v_left = {left, top_left, 10, length};
    SDL_Rect v_right = {right, top_left, 10, length};
    SDL_Rect h_left = {top_left, left, length, 10};
    SDL_Rect h_right = {top_left, right, length, 10};

    // rendering the grid
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &v_left);
    SDL_RenderFillRect(renderer, &v_right);
    SDL_RenderFillRect(renderer, &h_left);
    SDL_RenderFillRect(renderer, &h_right);

    // rendering the moves on the board
    for (Move move : m_moves)
    {
        int x = top_left + ((move.row + .13) * cell_size);
        int y = top_left + ((move.col + .13) * cell_size);
        SDL_Rect rect = {x, y, (int)(.85 * cell_size), (int)(.85 * cell_size)};
        RenderMove(renderer, move, rect);
    }
}

bool Board::IsGameOver()
{
    if (CheckColumns() || CheckRows() || CheckDiagonals() || m_moves.size() == 9)
        return true;
    else
        return false;
}

vector<int> Board::GetGridPosition(int x, int y, int cell_size, int low, int high)
{
    int row, col;
    if (x <= low + cell_size)
    {
        col = 0;
        if (y <= low + cell_size)
        {
            row = 0;
        }
        else if (y > low + cell_size && y <= low + (2 * cell_size))
        {
            row = 1;
        }
        else
        {
            row = 2;
        }
    }
    else if (x > low + cell_size && x <= low + (2 * cell_size))
    {
        col = 1;
        if (y <= low + cell_size)
        {
            row = 0;
        }
        else if (y > low + cell_size && y <= low + (2 * cell_size))
        {
            row = 1;
        }
        else
        {
            row = 2;
        }
    }
    else
    {
        col = 2;
        if (y <= low + cell_size)
        {
            row = 0;
        }
        else if (y > low + cell_size && y <= low + (2 * cell_size))
        {
            row = 1;
        }
        else
        {
            row = 2;
        }
    }
    return vector<int>{col, row};
}

void Board::RenderMove(SDL_Renderer *renderer, Move move, SDL_Rect rect)
{
    if (move.turn == Turn::X)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // top left corner to bottom right corner
        SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
        // top right corner to bottom left corner
        SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x, rect.y + rect.h);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        int center_x = rect.x + (rect.w / 2);
        int center_y = rect.y + (rect.h / 2);
        int radius = rect.w / 2;

        int x = radius - 1;
        int y = 0;
        int tx = 1;
        int ty = 1;
        int error = tx - (2 * radius);

        while (x >= y)
        {
            SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
            SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
            SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
            SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
            SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
            SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
            SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
            SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }
            else
            {
                --x;
                tx += 2;
                error += tx - (2 * radius);
            }
        }
    }
}

Turn Board::GetMove(int row, int col)
{
    for (Move move : m_moves)
    {
        if (move.row == row && move.col == col)
        {
            return move.turn;
        }
    }
    return Turn::None;
}

bool Board::CheckColumns()
{
    for (int i = 0; i < 3; i++)
    {
        if (GetMove(0, i) != Turn::None && (GetMove(0, i) == GetMove(1, i) && GetMove(1, i) == GetMove(2, i)))
            return true;
    }
    return false;
}
bool Board::CheckRows()
{
    for (int i = 0; i < 3; i++)
    {
        if (GetMove(i, 0) != Turn::None && (GetMove(i, 0) == GetMove(i, 1) && GetMove(i, 1) == GetMove(i, 2)))
            return true;
    }
    return false;
}

bool Board::CheckDiagonals()
{
    return (GetMove(0, 0) != Turn::None && (GetMove(0, 0) == GetMove(1, 1) && GetMove(1, 1) == GetMove(2, 2))) ||
           (GetMove(2, 0) != Turn::None && (GetMove(2, 0) == GetMove(1, 1) && GetMove(1, 1) == GetMove(0, 2)));
}