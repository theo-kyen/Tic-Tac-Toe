#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// texture wrapper class
class Texture
{
public:
    Texture(int width, int height) : m_texture(NULL), m_width(width), m_height(height) {}
    Texture() : Texture(0, 0) {}
    ~Texture()
    {
        Free();
    }

    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
    
    // renders the texture at the given coordinates
    void Render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = NULL);
    // deallocates texture
    void Free();

private:
    SDL_Texture *m_texture;
    int m_width, m_height;
};