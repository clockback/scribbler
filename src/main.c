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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./objects/graphics.h"
#include "./ECS/ECS.h"
#include "./ECS/Components.h"
#include "./objects/game.h"
#include "./groups.h"

int main(int argc, char *argv[]) {
    /* Sets up the game. */
    GamePtr game = (GamePtr) malloc(sizeof(Game));
    Game_init(game, 330, 230);

    Uint32 frame_start;
    int frame_time;
    const int fps = 60;
    const int frame_delay = 1000 / fps;

    /* Runs the main loop */
    while (game->running) {
    	frame_start = SDL_GetTicks();

		Game_handle_events(game);
    	Game_update(game);
    	Game_render(game);

    	frame_time = SDL_GetTicks() - frame_start;

    	if (frame_delay > frame_time) {
    		SDL_Delay(frame_delay - frame_time);
    	}
    }

    /* Clears the heap. */
    Game_clean(game);

    return 0;
}
