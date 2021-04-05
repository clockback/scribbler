#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* rend;
    int width;
    int height;
} Screen;

typedef Screen * ScreenPtr;

void Screen_init(ScreenPtr me, int width, int height);

#endif
