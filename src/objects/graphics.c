#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "./graphics.h"

void Screen_init(ScreenPtr me, int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error: SDL failed to initialize.\n");
        exit(-1);
    }

    me->window = SDL_CreateWindow(
        "Scribbler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 0
    );
    me->width = width;
    me->height = height;

    Uint32 render_flags = SDL_WINDOW_FULLSCREEN;
    me->rend = SDL_CreateRenderer(me->window, -1, render_flags);
}
