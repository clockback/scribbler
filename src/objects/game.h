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

#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

typedef struct Game Game;
typedef Game * GamePtr;

#include "./graphics.h"
#include "../ECS/ECS.h"
#include "./room.h"
#include "./font.h"
#include "./globals.h"
#include "../events/ScenarioManager.h"
#include "io/IoState.h"

struct Game {
    ScreenPtr screen;
    SDL_Renderer * rend;
    SDL_Event * event;
    bool running;
    SystemPtr sys;
    RoomPtr room;
    size_t no_rooms;
    RoomPtr * rooms;
    FontPtr font;
    ScenarioManagerPtr scenario_manager;
    GlobalsPtr globals;

    SDL_Texture * plain_cursor;
    SDL_Texture * interact_cursor;
    SDL_Rect * cursor_src_rect;
    SDL_Rect * cursor_dest_rect;
};

void Game_init(GamePtr me, int screen_width, int screen_height);
void Game_prepare(GamePtr me);
void Game_handle_events(GamePtr me);
void Game_update(GamePtr me);
void Game_handle_scenarios(GamePtr me);
void Game_render(GamePtr me);
void Game_clean(GamePtr me);
RoomPtr Game_find_room(GamePtr me, const char * name);
EntityPtr Game_find_entity(GamePtr me, char * name);

void Game_click(GamePtr me);

void quick_sort(EntityPtr * draw_order, int length);

#endif
