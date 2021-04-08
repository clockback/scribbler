#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

typedef struct {
    SDL_Window * window;
    SDL_Renderer * rend;
    SDL_Texture * main_tex;
    SDL_Rect * src_rect;
    SDL_Rect * dest_rect;
    int width;
    int height;
    int scale;
} Screen;

typedef Screen * ScreenPtr;

void Screen_init(ScreenPtr me, int width, int height, int scale);

#endif
