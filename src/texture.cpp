#include <iostream>
#include "../include/texture.hpp"
using namespace std;

void Texture::Render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip)
{
    // set render space
    SDL_Rect render_quad = {x, y, m_width, m_height};

    // set clip rendering dimensions
    if (clip != NULL)
    {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopy(renderer, m_texture, clip, &render_quad);
}

void Texture::Free()
{
    // free texture if it exists
    if (m_texture != NULL)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = NULL;
        m_width = 0;
        m_height = 0;
    }
}