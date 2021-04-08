#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "./graphics.h"

void Screen_init(ScreenPtr me, int width, int height, int scale) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error: SDL failed to initialize.\n");
        exit(-1);
    }

    me->window = SDL_CreateWindow(
        "Scribbler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width * scale, height * scale, 0
    );

    me->width = width;
    me->height = height;
    me->scale = scale;

    me->src_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    me->dest_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

    me->src_rect->x = 0;
    me->src_rect->y = 0;
    me->src_rect->w = width;
    me->src_rect->h = height;

    me->dest_rect->x = 0;
    me->dest_rect->y = 0;
    me->dest_rect->w = width * scale;
    me->dest_rect->h = height * scale;

    Uint32 render_flags = SDL_WINDOW_FULLSCREEN;
    me->rend = SDL_CreateRenderer(me->window, -1, render_flags);
    me->main_tex = SDL_CreateTexture(
		me->rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width,
		height
	);
    SDL_SetRenderTarget(me->rend, me->main_tex);
}
