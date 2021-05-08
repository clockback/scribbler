/**
 * Scribbler
 * Copyright (C) 2021 Elliot Paton-Simpson
 *
 * This file is part of Scribbler.
 *
 *  Scribbler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Scribbler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Scribbler.  If not, see <https://www.gnu.org/licenses/>.
 */

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

SDL_Texture * Screen_load_sprite(
	ScreenPtr me, char * path, int * width, int * height
) {
	unsigned char buffer[24];
	FILE * file_ptr;
	file_ptr = fopen(path, "rb");
	fread(buffer, sizeof(buffer), 1, file_ptr);

	*width = (int)(
		(unsigned char)(buffer[16]) << 24 |
		(unsigned char)(buffer[17]) << 16 |
	    (unsigned char)(buffer[18]) << 8 |
	    (unsigned char)(buffer[19])
	);

	*height = (int)(
		(unsigned char)(buffer[20]) << 24 |
		(unsigned char)(buffer[21]) << 16 |
		(unsigned char)(buffer[22]) << 8 |
		(unsigned char)(buffer[23])
	);

	SDL_Surface * temp_surface = IMG_Load(path);
	SDL_Texture * tex = SDL_CreateTextureFromSurface(
		me->rend, temp_surface
	);
	SDL_FreeSurface(temp_surface);

	return tex;
}
