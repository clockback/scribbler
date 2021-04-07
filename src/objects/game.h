#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

#include "./graphics.h"
#include "../ECS/ECS.h"
#include "./room.h"

typedef struct {
    Screen * screen;
    SDL_Renderer * rend;
    SDL_Event * event;
    bool running;
    SystemPtr sys;
    RoomPtr room;
} Game;

typedef Game * GamePtr;

void Game_init(GamePtr me, int screen_width, int screen_height);
void Game_prepare(GamePtr me);
void Game_handle_events(GamePtr me);
void Game_update(GamePtr me);
void Game_render(GamePtr me);
void Game_clean(GamePtr me);
void quick_sort(EntityPtr * draw_order, int length);

#endif
