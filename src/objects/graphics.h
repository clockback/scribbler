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
